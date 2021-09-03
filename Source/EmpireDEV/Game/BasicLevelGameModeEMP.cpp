// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "BasicLevelGameModeEMP.h"
#include "../Level/EMPGridSquare.h"
#include "../Combat/EMPCombatPlayer.h"
#include "../Combat/EMPSquad.h"
#include "../Combat/EMPCombatUnit.h"
#include "../Combat/CombatActions/EMPMovementAction.h"

void ABasicLevelGameModeEMP::BeginPlay()
{
	Super::BeginPlay();

	SpawnGrid();

	FriendlyCombatPlayer = GetWorld()->SpawnActor<AEMPCombatPlayer>();
	for (int squadNum = 0; squadNum < 4; squadNum++)
	{
		AEMPSquad* friendlySquad = FriendlyCombatPlayer->CreateSquad();
		friendlySquad->SetStartingAreaCoordinate(FIntPoint(0, squadNum));
		for (int i = 0; i < 5; i++)
		{
			AEMPCombatUnit* combatUnit = friendlySquad->CreateCombatUnit(CombatUnitClass);
			FIntPoint gridLocation = FIntPoint(0, i + squadNum * GridAreaSize);
			combatUnit->InitializeToGridSquare(gridLocation);
		}
	}
	

	EnemyCombatPlayer = GetWorld()->SpawnActor<AEMPCombatPlayer>();
	for (int squadNum = 0; squadNum < 1; squadNum++)
	{
		AEMPSquad* enemySquad = EnemyCombatPlayer->CreateSquad();
		enemySquad->SetStartingAreaCoordinate(FIntPoint(4, squadNum));
		for (int i = 0; i < 5; i++)
		{
			AEMPCombatUnit* combatUnit = enemySquad->CreateCombatUnit(CombatUnitClass);
			FIntPoint gridLocation = FIntPoint(4 * GridAreaSize, i + squadNum * GridAreaSize);
			combatUnit->InitializeToGridSquare(gridLocation);
		}
	}
}

void ABasicLevelGameModeEMP::SpawnGrid()
{
	
	for (int j = 0; j < GridDimensions.Y; j++)
	{
		for (int i = 0; i < GridDimensions.X; i++)
		{
			FVector location = GetWorldLocationFromGridLocation(FIntPoint(i, j));
			AEMPGridSquare* GridSquareActor = GetWorld()->SpawnActor<AEMPGridSquare>(GridSquareClass, location, FRotator(0, 0, 0));
			GridSquareActor->SetActorScale3D(FVector(SingleGridSquareSize.X, SingleGridSquareSize.Y, 1));
			GridSquareActor->InitializeGridSquare(FIntPoint(i, j), SingleGridSquareSize);
			GridSquareActor->OnGridSquareClicked.AddUniqueDynamic(this, &ABasicLevelGameModeEMP::HandleGridSquareClicked);
			GridSquareActor->OnGridSquareBeginCursorOver.AddUniqueDynamic(this, &ABasicLevelGameModeEMP::HandleGridSquareHovered);
			GridSquareActor->OnGridSquareEndCursorOver.AddUniqueDynamic(this, &ABasicLevelGameModeEMP::HandleGridSquareUnhovered);
			GridSquares.Add(GridSquareActor);
		}
	}
}

void ABasicLevelGameModeEMP::HandleGridSquareClicked(AEMPGridSquare* inGridSquare)
{
	FGridDataEMP gridData = GetGridDataForSquare(inGridSquare);
	if (IsGridConditionMetForGivenState(CurrentGameState, gridData))
	{
		if (CurrentGameState == EBasicLevelGameStateEMP::GS_SELECTING_SQUAD)
		{
			// Transition to squad ordering
			HandleSetGridAreaHighlighted(gridData.areaCoordinate, false);
			GridAreaEditing = gridData.areaCoordinate;

			SelectSquad(gridData.squadAtArea);
			SetCurrentGameState(EBasicLevelGameStateEMP::GS_ORDERING_SQUAD);
		}
		else if (CurrentGameState == EBasicLevelGameStateEMP::GS_ORDERING_SQUAD)
		{
			// No actions to take
		}
		else if (CurrentGameState == EBasicLevelGameStateEMP::GS_REARRANGING_SQUAD)
		{			
			SelectUnit(gridData.unitAtCoordinate);
			inGridSquare->SetHighlighted(false);
			SetCurrentGameState(EBasicLevelGameStateEMP::GS_MOVING_UNIT);
		}
		else if (CurrentGameState == EBasicLevelGameStateEMP::GS_MOVING_UNIT)
		{
			SelectedUnit->MoveToGridSquare(gridData.gridCoordinate, false);
			DeselectUnit();
			SelectedSquad->SetCurrentSquadState(ESquadStateEMP::SS_REARRANGED);
			SetCurrentGameState(EBasicLevelGameStateEMP::GS_REARRANGING_SQUAD);
		}
		else if (CurrentGameState == EBasicLevelGameStateEMP::GS_MOVING_SQUAD)
		{
			// Select grid areas that aren't the current space
			QueueAction_MoveSquad(SelectedSquad, gridData.areaCoordinate);
			SelectedSquad->SetCurrentSquadState(ESquadStateEMP::SS_MOVE_QUEUED);

			DeselectSquad();

			HandleSetGridAreaHighlighted(gridData.areaCoordinate, false);
			SetCurrentGameState(EBasicLevelGameStateEMP::GS_SELECTING_SQUAD);
			
		}
	}
}

void ABasicLevelGameModeEMP::HandleGridSquareHovered(AEMPGridSquare* inGridSquare)
{
	HoveredGridSquare = inGridSquare;
	HandleSetGridHighlighted(inGridSquare, true);
}

void ABasicLevelGameModeEMP::HandleGridSquareUnhovered(AEMPGridSquare* inGridSquare)
{
	HoveredGridSquare = nullptr;
	HandleSetGridHighlighted(inGridSquare, false);
}

void ABasicLevelGameModeEMP::HandleSetGridHighlighted(AEMPGridSquare* inGridSquare, bool isHighlighted)
{
	FGridDataEMP gridData = GetGridDataForSquare(inGridSquare);
	if (IsGridConditionMetForGivenState(CurrentGameState, gridData))
	{
		if (CurrentGameState == EBasicLevelGameStateEMP::GS_SELECTING_SQUAD)
		{
			// Highlight grid area
			HandleSetGridAreaHighlighted(gridData.areaCoordinate, isHighlighted);
		}
		else if (CurrentGameState == EBasicLevelGameStateEMP::GS_ORDERING_SQUAD)
		{
			// Empty response
		}
		else if (CurrentGameState == EBasicLevelGameStateEMP::GS_REARRANGING_SQUAD)
		{
			// Highlight on combat units that can be rearranged
			inGridSquare->SetHighlighted(isHighlighted);
		}
		else if (CurrentGameState == EBasicLevelGameStateEMP::GS_MOVING_UNIT)
		{
			// Highlight spaces that the unit can move to
			inGridSquare->SetHighlighted(isHighlighted);
		}
		else if (CurrentGameState == EBasicLevelGameStateEMP::GS_MOVING_SQUAD)
		{
			// Highlight grid areas that aren't the current space
			HandleSetGridAreaHighlighted(gridData.areaCoordinate, isHighlighted);
		}
	}
}

void ABasicLevelGameModeEMP::HandleSetGridAreaHighlighted(FIntPoint areaCoordinate, bool isHighlighted)
{
	TArray<AEMPGridSquare*> gridSquaresToHighlight = GetGridSquaresInArea(areaCoordinate);

	for (AEMPGridSquare* gridSquare : gridSquaresToHighlight)
	{
		gridSquare->SetHighlighted(isHighlighted);
	}
}

bool ABasicLevelGameModeEMP::IsGridCoordinateInAreaCoordinate(FIntPoint gridCoordinate, FIntPoint areaCoordinate)
{
	bool bIsInX = gridCoordinate.X >= areaCoordinate.X * GridAreaSize && gridCoordinate.X < (areaCoordinate.X + 1) * GridAreaSize;
	bool bIsInY = gridCoordinate.Y >= areaCoordinate.Y * GridAreaSize && gridCoordinate.Y < (areaCoordinate.Y + 1)* GridAreaSize;

	return bIsInX && bIsInY;
}

FGridDataEMP ABasicLevelGameModeEMP::GetGridDataForSquare(AEMPGridSquare* inGridSquare)
{
	FGridDataEMP data;
	data.gridCoordinate = inGridSquare->GetGridCoordinate();
	data.areaCoordinate = GetAreaCoordinateOfGridCoordinate(data.gridCoordinate);
	data.unitAtCoordinate = nullptr;
	if (SelectedSquad) data.unitAtCoordinate = SelectedSquad->GetCombatUnitAtGridCoordinate(data.gridCoordinate);

	data.squadAtArea = GetFriendlySquadAtAreaCoordinate(data.areaCoordinate);

	return data;
}


AEMPGridSquare* ABasicLevelGameModeEMP::GetGridSquareAtCoordinate(FIntPoint gridCoordinate) const
{
	int32 transformedIndex = GridDimensions.X * gridCoordinate.Y + gridCoordinate.X;
	check(transformedIndex < GridSquares.Num()); // Sanity check array index

	return GridSquares[transformedIndex];
}


FIntPoint ABasicLevelGameModeEMP::GetAreaCoordinateOfGridCoordinate(FIntPoint gridCoordinate) const
{
	return FIntPoint(gridCoordinate.X / GridAreaSize, gridCoordinate.Y / GridAreaSize);
}

TArray<AEMPGridSquare*> ABasicLevelGameModeEMP::GetGridSquaresInArea(FIntPoint areaCoordinate) const
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

bool ABasicLevelGameModeEMP::IsGridConditionMetForGivenState(EBasicLevelGameStateEMP givenState, FGridDataEMP gridData)
{
	if (CurrentGameState == EBasicLevelGameStateEMP::GS_SELECTING_SQUAD)
	{
		// Valid if there is a squad at the current area
		return gridData.squadAtArea != nullptr;
	}
	else if (CurrentGameState == EBasicLevelGameStateEMP::GS_ORDERING_SQUAD)
	{
		// Nothing is interactable in this state
	}
	else if (CurrentGameState == EBasicLevelGameStateEMP::GS_REARRANGING_SQUAD)
	{
		check(SelectedSquad);
		return gridData.unitAtCoordinate != nullptr;
	}
	else if (CurrentGameState == EBasicLevelGameStateEMP::GS_MOVING_UNIT)
	{
		check(SelectedSquad && SelectedUnit);
		return !gridData.unitAtCoordinate && IsGridCoordinateInAreaCoordinate(gridData.gridCoordinate, SelectedSquad->GetCurrentAreaCoordinate());
	}
	else if (CurrentGameState == EBasicLevelGameStateEMP::GS_MOVING_SQUAD)
	{
		// Grid areas that are exactly one space away are valid
		check(SelectedSquad);
		FIntPoint selectedSquadAreaCoordinate = SelectedSquad->GetCurrentAreaCoordinate();
		int32 gridDistance = FMath::Abs(gridData.areaCoordinate.X - selectedSquadAreaCoordinate.X) + FMath::Abs(gridData.areaCoordinate.Y - selectedSquadAreaCoordinate.Y);
		return gridDistance == 1;
	}
	return false;
}

void ABasicLevelGameModeEMP::SelectSquad(AEMPSquad* squadToSelect)
{
	SelectedSquad = squadToSelect;
	OnSquadSelected.Broadcast(SelectedSquad);
}

void ABasicLevelGameModeEMP::DeselectSquad()
{
	SelectedSquad = nullptr;
	OnSquadDeselected.Broadcast();
}

void ABasicLevelGameModeEMP::SelectUnit(AEMPCombatUnit* unitToSelect)
{
	SelectedUnit = unitToSelect;
}

void ABasicLevelGameModeEMP::DeselectUnit()
{
	SelectedUnit = nullptr;
}

void ABasicLevelGameModeEMP::EnterRearrangingSquadState()
{
	if (CurrentGameState == EBasicLevelGameStateEMP::GS_ORDERING_SQUAD)
	{
		SetCurrentGameState(EBasicLevelGameStateEMP::GS_REARRANGING_SQUAD);
	}
}

void ABasicLevelGameModeEMP::EnterMovingSquadState()
{
	if (CurrentGameState == EBasicLevelGameStateEMP::GS_ORDERING_SQUAD)
	{
		SetCurrentGameState(EBasicLevelGameStateEMP::GS_MOVING_SQUAD);
	}
}

void ABasicLevelGameModeEMP::QueueAction_MoveSquad(AEMPSquad* squadToMove, FIntPoint areaCoordinateOfDestination)
{
	UEMPMovementAction* movementAction = NewObject<UEMPMovementAction>();
	movementAction->SetSquad(squadToMove);
	movementAction->SetDestinationAreaCoordinate(areaCoordinateOfDestination);

	CombatActionsQueue.Add(movementAction);
	OnActionQueued.Broadcast(FString::Printf(TEXT("Moving squad %s to location %s"), *squadToMove->GetCurrentAreaCoordinate().ToString(), *areaCoordinateOfDestination.ToString()));
}

AEMPSquad* ABasicLevelGameModeEMP::GetFriendlySquadAtAreaCoordinate(FIntPoint areaCoordinate)
{
	for (AEMPSquad* squad : FriendlyCombatPlayer->GetSquads())
	{
		if (squad->GetCurrentAreaCoordinate() == areaCoordinate)
		{
			return squad;
		}
	}

	// If area coordinate is empty
	return nullptr;
}

AEMPSquad* ABasicLevelGameModeEMP::GetEnemySquadAtAreaCoordinate(FIntPoint areaCoordinate)
{
	for (AEMPSquad* squad : EnemyCombatPlayer->GetSquads())
	{
		if (squad->GetCurrentAreaCoordinate() == areaCoordinate)
		{
			return squad;
		}
	}

	// If area coordinate is empty
	return nullptr;
}

FVector ABasicLevelGameModeEMP::GetWorldLocationFromGridLocation(FIntPoint gridLocation)
{
	return FVector(SingleGridSquareSize.X * gridLocation.X, SingleGridSquareSize.Y * gridLocation.Y, GridBaseHeight);
}

EBasicLevelGameStateEMP ABasicLevelGameModeEMP::GetCurrentGameState() const
{
	return CurrentGameState;
}

void ABasicLevelGameModeEMP::SetCurrentGameState(EBasicLevelGameStateEMP newState)
{
	CurrentGameState = newState;
	OnGameStateChanged.Broadcast(CurrentGameState);
}

void ABasicLevelGameModeEMP::HandleCancelActionPressed()
{
	if (CurrentGameState == EBasicLevelGameStateEMP::GS_SELECTING_SQUAD)
	{
		// Empty response
	}
	else if (CurrentGameState == EBasicLevelGameStateEMP::GS_ORDERING_SQUAD)
	{
		DeselectSquad();

		// Exit squad ordering, enter squad selecting
		if (HoveredGridSquare)
		{
			FIntPoint areaCoordinate = GetAreaCoordinateOfGridCoordinate(HoveredGridSquare->GetGridCoordinate());
			AEMPSquad* squadAtArea = GetFriendlySquadAtAreaCoordinate(areaCoordinate);
			if (squadAtArea)
			{
				// Highlight grid area
				HandleSetGridAreaHighlighted(areaCoordinate, true);
			}
		}
		SetCurrentGameState(EBasicLevelGameStateEMP::GS_SELECTING_SQUAD);
	}
	else if (CurrentGameState == EBasicLevelGameStateEMP::GS_REARRANGING_SQUAD)
	{
		// Exit squad rearranging, enter squad ordering
		if (HoveredGridSquare)
		{
			HoveredGridSquare->SetHighlighted(false);
		}
		SetCurrentGameState(EBasicLevelGameStateEMP::GS_ORDERING_SQUAD);
	}
	else if (CurrentGameState == EBasicLevelGameStateEMP::GS_MOVING_UNIT)
	{
		if (HoveredGridSquare)
		{
			FGridDataEMP gridData = GetGridDataForSquare(HoveredGridSquare);
			HoveredGridSquare->SetHighlighted(gridData.unitAtCoordinate != nullptr);
		}
		SetCurrentGameState(EBasicLevelGameStateEMP::GS_REARRANGING_SQUAD);

	}
	else if (CurrentGameState == EBasicLevelGameStateEMP::GS_MOVING_SQUAD)
	{
		// Exit squad moving, enter squad ordering
		if (HoveredGridSquare)
		{
			FIntPoint areaCoordinate = GetAreaCoordinateOfGridCoordinate(HoveredGridSquare->GetGridCoordinate());
			HandleSetGridAreaHighlighted(areaCoordinate, false);
		}
		SetCurrentGameState(EBasicLevelGameStateEMP::GS_ORDERING_SQUAD);
	}
}

void ABasicLevelGameModeEMP::EnterResolvingOrdersState()
{
	if (CurrentGameState == EBasicLevelGameStateEMP::GS_SELECTING_SQUAD)
	{
		SetCurrentGameState(EBasicLevelGameStateEMP::GS_RESOLVING_ORDERS);
		ResolveCurrentCombatAction();
	}
}

void ABasicLevelGameModeEMP::ResolveCurrentCombatAction()
{
	if (CombatActionsQueue.Num() > 0)
	{
		UEMPCombatAction* action = CombatActionsQueue[0];
		action->ExecuteAction(this);
		CombatActionsQueue.RemoveAt(0);
		OnActionQueuePopped.Broadcast();
	}
	else
	{
		// All actions are finished, move on to next state
		for (AEMPSquad* squad : FriendlyCombatPlayer->GetSquads())
		{
			squad->SetCurrentSquadState(ESquadStateEMP::SS_AWAITING_ORDERS);
		}

		SetCurrentGameState(EBasicLevelGameStateEMP::GS_SELECTING_SQUAD);
	}
}


void ABasicLevelGameModeEMP::HandleCombatUnitAnimationFinished()
{
	if (CurrentGameState == EBasicLevelGameStateEMP::GS_RESOLVING_ORDERS)
	{
		ResolveCurrentCombatAction();
	}
}
