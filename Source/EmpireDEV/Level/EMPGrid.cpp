// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPGrid.h"
#include "EMPGridSquare.h"
#include "../Combat/EMPCombatUnit.h"

AEMPGrid::AEMPGrid()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEMPGrid::BeginPlay()
{
	Super::BeginPlay();

	SpawnGrid();
}

void AEMPGrid::SpawnGrid()
{
	for (int j = 0; j < GridDimensions.Y; j++)
	{
		for (int i = 0; i < GridDimensions.X; i++)
		{
			FVector location = GetWorldLocationFromGridLocation(FIntPoint(i, j));
			AEMPGridSquare* GridSquareActor = GetWorld()->SpawnActor<AEMPGridSquare>(GridSquareClass, location, FRotator(0, 0, 0));
			GridSquareActor->SetActorScale3D(FVector(SingleGridSquareSize.X, SingleGridSquareSize.Y, 1));
			GridSquareActor->InitializeGridSquare(FIntPoint(i, j));
			GridSquareActor->OnGridSquareClicked.AddUniqueDynamic(this, &AEMPGrid::HandleGridSquareClicked);
			GridSquareActor->OnGridSquareBeginCursorOver.AddUniqueDynamic(this, &AEMPGrid::HandleGridSquareHovered);
			GridSquareActor->OnGridSquareEndCursorOver.AddUniqueDynamic(this, &AEMPGrid::HandleGridSquareUnhovered);
			GridSquares.Add(GridSquareActor);
		}
	}
}

void AEMPGrid::HandleGridSquareHovered(AEMPGridSquare* inGridSquare)
{
	HoveredGridSquare = inGridSquare;
}

void AEMPGrid::HandleGridSquareUnhovered(AEMPGridSquare* inGridSquare)
{
	HoveredGridSquare = nullptr;
}

void AEMPGrid::HandleGridSquareClicked(AEMPGridSquare* inGridSquare)
{
	// No default behavior
}

void AEMPGrid::HandleSetGridHighlighted(class AEMPGridSquare* inGridSquare, bool isHighlighted)
{
	// No default behavior
}

void AEMPGrid::HandleSetGridAreaHighlighted(FIntPoint areaCoordinate, bool isHighlighted)
{
	TArray<AEMPGridSquare*> gridSquaresToHighlight = GetGridSquaresInArea(areaCoordinate);

	for (AEMPGridSquare* gridSquare : gridSquaresToHighlight)
	{
		gridSquare->SetHighlighted(isHighlighted);
	}
}

bool AEMPGrid::IsGridCoordinateInAreaCoordinate(FIntPoint gridCoordinate, FIntPoint areaCoordinate)
{
	bool bIsInX = gridCoordinate.X >= areaCoordinate.X * GridAreaSize && gridCoordinate.X < (areaCoordinate.X + 1)* GridAreaSize;
	bool bIsInY = gridCoordinate.Y >= areaCoordinate.Y * GridAreaSize && gridCoordinate.Y < (areaCoordinate.Y + 1)* GridAreaSize;

	return bIsInX && bIsInY;
}

AEMPGridSquare* AEMPGrid::GetGridSquareAtCoordinate(FIntPoint gridCoordinate) const
{
	int32 transformedIndex = GridDimensions.X * gridCoordinate.Y + gridCoordinate.X;
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

FVector AEMPGrid::GetWorldLocationFromGridLocation(FIntPoint gridLocation)
{
	return FVector(SingleGridSquareSize.X * gridLocation.X, SingleGridSquareSize.Y * gridLocation.Y, GridBaseHeight);
}
