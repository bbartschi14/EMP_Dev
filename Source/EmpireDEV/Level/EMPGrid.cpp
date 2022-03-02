// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPGrid.h"
#include "EMPGridSquare.h"
#include "../Combat/EMPCombatUnit.h"
#include "EMPGridAreaHighlight.h"
#include "EMPLandscape.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AEMPGrid::AEMPGrid()
{
	PrimaryActorTick.bCanEverTick = true;
}

int32 AEMPGrid::GetGridDimension() const
{
	return GridDimensions;
}

void AEMPGrid::SetGridDimension(int32 InDimension)
{
	GridDimensions = InDimension;
}

TArray<int32> AEMPGrid::GetHeights() const
{
	return Heights;
}

void AEMPGrid::SetHeights(TArray<int32> InHeights)
{
	Heights = InHeights;
}

int32 AEMPGrid::GetHeightOfAreaCoordinate(FIntPoint areaCoordinate) const
{
	int32 index = areaCoordinate.Y * GridDimensions + areaCoordinate.X;
	if (Heights.IsValidIndex(index))
	{
		return Heights[index];
	}
	return 0;
}

void AEMPGrid::GetAllAreaCoordinates(TArray<FIntPoint>& OutAreaCoordinates) const
{
	for (int i = 0; i < GridAreaHighlights.Num(); i++)
	{
		OutAreaCoordinates.Add(FIntPoint(i % GridDimensions, i / GridDimensions));
	}
}

AEMPGridAreaHighlight* AEMPGrid::GetGridAreaHighlightAtCoordinate(FIntPoint areaCoordinate) const
{
	int32 transformedCoord = areaCoordinate.Y * GridDimensions + areaCoordinate.X;
	if (GridAreaHighlights.IsValidIndex(transformedCoord))
	{
		return GridAreaHighlights[transformedCoord];
	}
	return nullptr;
}

void AEMPGrid::Get2DBounds(FVector2D& Min, FVector2D& Max) const
{
	Min = FVector2D(0, 0);
	float gridAreaSize = SingleGridSquareSize * 5 + (2 * BorderOffset);
	Max = FVector2D(gridAreaSize * GridDimensions);
}

void AEMPGrid::BeginPlay()
{
	Super::BeginPlay();
	SpawnGrid();

	// Populate reference arrays

	GridSquares.SetNum(GridDimensions * GridDimensions * 5 * 5);
	TArray<AActor*> gridSquares;
	GridSquareRoot->GetAttachedActors(gridSquares);
	for (int i = 0; i < gridSquares.Num(); i++)
	{
		AEMPGridSquare* square = Cast<AEMPGridSquare>(gridSquares[i]);
		int32 transformedIndex = GridDimensions * 5 * square->GetGridCoordinate().Y + square->GetGridCoordinate().X;
		GridSquares[transformedIndex] = square;
	}

	TArray<AActor*> gridAreas;
	GridAreaHighlightRoot->GetAttachedActors(gridAreas);
	GridAreaHighlights.SetNum(gridAreas.Num());
	for (int i = 0; i < gridAreas.Num(); i++)
	{
		AEMPGridAreaHighlight* highlight = Cast<AEMPGridAreaHighlight>(gridAreas[i]);
		int32 transformedCoord = highlight->AreaCoordinate.Y * GridDimensions + highlight->AreaCoordinate.X;
		GridAreaHighlights[transformedCoord] = highlight;
	}
}

void AEMPGrid::SpawnLandscape()
{
	TArray<AActor*> landscapes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEMPLandscape::StaticClass(), landscapes);
	if (landscapes.Num() > 0)
	{
		Landscape = Cast<AEMPLandscape>(landscapes[0]);
		Landscape->SpawnLandscape(GridDimensions, Heights, SingleGridSquareSize * 5, BorderOffset);
	}
}

void AEMPGrid::SpawnGrid()
{
	// Cleanup
	if (GridSquareRoot)
	{
		TArray<AActor*> gridSquares;
		GridSquareRoot->GetAttachedActors(gridSquares);
		for (AActor* gridSquare : gridSquares)
		{
			gridSquare->Destroy();
		}
	}
	else
	{
		GridSquareRoot = GetWorld()->SpawnActor<AActor>(ActorRootClass, FTransform());
		GridSquareRoot->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
	
	if (GridAreaHighlightRoot)
	{
		TArray<AActor*> children;
		GridAreaHighlightRoot->GetAttachedActors(children);
		for (AActor* child : children)
		{
			child->Destroy();
		}
	}
	else
	{
		GridAreaHighlightRoot = GetWorld()->SpawnActor<AActor>(ActorRootClass, FTransform());
		GridAreaHighlightRoot->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}

	SpawnLandscape();

	float gridAreaSize = SingleGridSquareSize * 5 + (2 * BorderOffset);

	for (int gridY = 0; gridY < GridDimensions; gridY++)
	{
		for (int gridX = 0; gridX < GridDimensions; gridX++)
		{
			for (int j = 0; j < 5; j++)
			{
				for (int i = 0; i < 5; i++)
				{
					int32 currentX = gridX * 5 + i;
					int32 currentY = gridY * 5 + j;

					FVector gridAreaOrigin = FVector(gridAreaSize * gridX, gridAreaSize * gridY, 0);
					// Raycast down into landscape mesh
					float cornerOffset = .35;
					FVector start = FVector(gridAreaOrigin.X + (SingleGridSquareSize * (i + cornerOffset) + BorderOffset), gridAreaOrigin.Y + (SingleGridSquareSize * (j + cornerOffset) + BorderOffset), 10000.0f);
					FVector end = start + FVector(0, 0, -1) * 20000.0f;

					FHitResult outHit;
					FCollisionQueryParams collisionParams;

					//DrawDebugLine(GetWorld(), start, end, FColor::Green, true, 1, 0, 1);

					if (GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, collisionParams))
					{
						FVector location = FVector(start.X, start.Y, outHit.ImpactPoint.Z);
						AEMPGridSquare* GridSquareActor = GetWorld()->SpawnActor<AEMPGridSquare>(GridSquareClass, location, FRotator(0, 0, 0));
						GridSquareActor->AttachToActor(GridSquareRoot, FAttachmentTransformRules::KeepRelativeTransform);
						GridSquareActor->SetActorScale3D(FVector(SingleGridSquareSize, SingleGridSquareSize, 1));
						GridSquareActor->InitializeGridSquare(FIntPoint(currentX, currentY));
						GridSquareActor->OnGridSquareClicked.AddUniqueDynamic(this, &AEMPGrid::HandleGridSquareClicked);
						GridSquareActor->OnGridSquareRightClicked.AddUniqueDynamic(this, &AEMPGrid::HandleGridSquareRightClicked);
						GridSquareActor->OnGridSquareBeginCursorOver.AddUniqueDynamic(this, &AEMPGrid::HandleGridSquareHovered);
						GridSquareActor->OnGridSquareEndCursorOver.AddUniqueDynamic(this, &AEMPGrid::HandleGridSquareUnhovered);						

						// Spawn Area highlight at center point
						if ((i - 2) % 5 == 0 && (j - 2) % 5 == 0 && bShouldSpawnAreaHighlights)
						{
							AEMPGridAreaHighlight* GridAreaHighlightActor = GetWorld()->SpawnActor<AEMPGridAreaHighlight>(GridAreaHighlightClass, location, FRotator(0, 0, 0));
							GridAreaHighlightActor->AttachToActor(GridAreaHighlightRoot, FAttachmentTransformRules::KeepRelativeTransform);
							GridAreaHighlightActor->AreaCoordinate = FVector2D(gridX, gridY);
						}

					}
				}
			}
		}
	}
	RefreshGridVisuals();

}

void AEMPGrid::HandleGridSquareHovered(AEMPGridSquare* inGridSquare)
{
	HoveredGridSquare = inGridSquare;
	OnGridSquareHovered(inGridSquare);
}

void AEMPGrid::HandleGridSquareUnhovered(AEMPGridSquare* inGridSquare)
{
	HoveredGridSquare = nullptr;
	OnGridSquareUnhovered(inGridSquare);
}

void AEMPGrid::HandleGridSquareClicked(AEMPGridSquare* inGridSquare)
{
	OnGridSquareClicked(inGridSquare);
}

void AEMPGrid::HandleGridSquareRightClicked(AEMPGridSquare* inGridSquare)
{
	OnGridSquareRightClicked(inGridSquare);
}

void AEMPGrid::HandleSetGridHighlighted(class AEMPGridSquare* inGridSquare, bool isHighlighted)
{
	// No default behavior
}

void AEMPGrid::HandleSetGridAreaHighlighted(FIntPoint areaCoordinate, bool isHighlighted)
{
	AEMPGridAreaHighlight* areaHighlight = GetGridAreaHighlightAtCoordinate(areaCoordinate);
	areaHighlight->SetHovered(isHighlighted);
	//TArray<AEMPGridSquare*> gridSquaresToHighlight = GetGridSquaresInArea(areaCoordinate);

	//for (AEMPGridSquare* gridSquare : gridSquaresToHighlight)
	//{
	//	gridSquare->SetHighlighted(isHighlighted);
	//}
}

void AEMPGrid::HandleSetGridAreaSelected(FIntPoint areaCoordinate, bool isSelected)
{
	AEMPGridAreaHighlight* areaHighlight = GetGridAreaHighlightAtCoordinate(areaCoordinate);
	areaHighlight->SetSelected(isSelected);

	//TArray<AEMPGridSquare*> gridSquaresToSelect = GetGridSquaresInArea(areaCoordinate);

	//for (AEMPGridSquare* gridSquare : gridSquaresToSelect)
	//{
	//	gridSquare->SetSelected(isSelected);
	//}
}

bool AEMPGrid::IsGridCoordinateInAreaCoordinate(FIntPoint gridCoordinate, FIntPoint areaCoordinate)
{
	bool bIsInX = gridCoordinate.X >= areaCoordinate.X * GridAreaSize && gridCoordinate.X < (areaCoordinate.X + 1)* GridAreaSize;
	bool bIsInY = gridCoordinate.Y >= areaCoordinate.Y * GridAreaSize && gridCoordinate.Y < (areaCoordinate.Y + 1)* GridAreaSize;

	return bIsInX && bIsInY;
}

AEMPGridSquare* AEMPGrid::GetGridSquareAtCoordinate(FIntPoint gridCoordinate) const
{
	int32 transformedIndex = GridDimensions * 5 * gridCoordinate.Y + gridCoordinate.X;
	check(transformedIndex < GridSquares.Num()); // Sanity check array index

	return GridSquares[transformedIndex];
}

AEMPCombatUnit* AEMPGrid::GetCombatUnitAtCoordinate(FIntPoint gridCoordinate) const
{
	for (AEMPCombatUnit* unitToCheck : CombatUnits)
	{
		if (unitToCheck->GetGridCoordinate() == gridCoordinate)
		{
			return unitToCheck;
		}
	}
	return nullptr;
}


FIntPoint AEMPGrid::GetAreaCoordinateOfGridCoordinate(FIntPoint gridCoordinate) const
{
	return FIntPoint(gridCoordinate.X / GridAreaSize, gridCoordinate.Y / GridAreaSize);
}

TArray<AEMPGridSquare*> AEMPGrid::GetGridSquaresInArea(FIntPoint areaCoordinate) const
{
	TArray<AEMPGridSquare*> gridSquaresInArea;
	int32 startingX = areaCoordinate.X * GridAreaSize;
	int32 startingY = areaCoordinate.Y * GridAreaSize;

	for (int i = startingX; i < startingX + GridAreaSize; i++)
	{
		for (int j = startingY; j < startingY + GridAreaSize; j++)
		{
			gridSquaresInArea.Add(GetGridSquareAtCoordinate(FIntPoint(i, j)));
		}
	}

	return gridSquaresInArea;
}

FVector AEMPGrid::GetWorldLocationFromGridLocation(FIntPoint gridLocation) const
{
	return GridSquares[GridDimensions * 5 * gridLocation.Y + gridLocation.X]->GetActorLocation();
	//return FVector(SingleGridSquareSize * gridLocation.X + SingleGridSquareSize/2, SingleGridSquareSize * gridLocation.Y + SingleGridSquareSize / 2, GridBaseHeight);
}

AEMPCombatUnit* AEMPGrid::GetCombatUnitFromData(UEMPCombatUnitData* combatUnitData) const
{
	for (AEMPCombatUnit* unit : CombatUnits)
	{
		if (unit->GetCombatUnitData() == combatUnitData)
		{
			return unit;
		}
	}

	return nullptr;
}
