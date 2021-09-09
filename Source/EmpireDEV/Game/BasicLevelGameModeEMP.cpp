// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "BasicLevelGameModeEMP.h"
#include "../Level/EMPGridSquare.h"
#include "../Combat/EMPCombatPlayer.h"
#include "../Combat/EMPSquad.h"
#include "../Combat/EMPCombatUnit.h"
#include "../Combat/CombatActions/EMPMovementAction.h"
#include "../Combat/Structs/FEMPSquadData.h"
#include "../Combat/Structs/FEMPCombatUnitData.h"

void ABasicLevelGameModeEMP::BeginPlay()
{
	Super::BeginPlay();

	SpawnGrid();

	FriendlyCombatPlayer = GetWorld()->SpawnActor<AEMPCombatPlayer>();
	EnemyCombatPlayer = GetWorld()->SpawnActor<AEMPCombatPlayer>();

	LoadFriendlySquads(FriendlyCombatPlayer);
	LoadEnemySquads(EnemyCombatPlayer);
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

void ABasicLevelGameModeEMP::LoadSquad(AEMPCombatPlayer* owningCombatPlayer, UEMPSquadData* squadData, FIntPoint areaCoordinate)
{
	AEMPSquad* squad = owningCombatPlayer->CreateSquad();
	squad->SetStartingAreaCoordinate(areaCoordinate);
	squad->SetSquadData(squadData);

	for (UEMPCombatUnitData* combatUnitData : squadData->CombatUnitsInSquad)
	{
		AEMPCombatUnit* combatUnit = squad->CreateCombatUnit(CombatUnitClass);
		FIntPoint gridLocation = FIntPoint(combatUnitData->DesiredLocationX + areaCoordinate.X * 5, combatUnitData->DesiredLocationY + areaCoordinate.Y * 5);
		combatUnit->InitializeToGridSquare(gridLocation);
		combatUnit->InitializeCombatUnitData(combatUnitData);
	}
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
	else if (CurrentGameState == EBasicLevelGameStateEMP::GS_RESOLVING_COMBAT)
	{
		TryStartCombatRound();
	}
	else if (CurrentGameState == EBasicLevelGameStateEMP::GS_RESET_SQUAD_AFTER_COMBAT)
	{
		EndCombat();
	}
}

void ABasicLevelGameModeEMP::SimulateCombatDamage(AEMPSquad* squadOne, AEMPSquad* squadTwo)
{
	FIntPoint squadOneAttackingDirection = squadTwo->GetCurrentAreaCoordinate() - squadOne->GetCurrentAreaCoordinate();
	FIntPoint squadTwoAttackingDirection = squadOneAttackingDirection * -1;

	// Calculate who attacks who and damage
	SimulateSquadAttacks(squadOne, squadTwo, squadOneAttackingDirection);
	SimulateSquadAttacks(squadTwo, squadOne, squadTwoAttackingDirection);

	// Update unit states
	for (AEMPCombatUnit* unit : squadOne->GetCombatUnitsInSquad())
	{
		unit->ResolvedCachedDamage();
	}

	for (AEMPCombatUnit* unit : squadTwo->GetCombatUnitsInSquad())
	{
		unit->ResolvedCachedDamage();
	}
}

bool ABasicLevelGameModeEMP::UpdateCombatMovement(AEMPSquad* squadOne, AEMPSquad* squadTwo)
{
	FIntPoint squadOneAttackingDirection = squadTwo->GetCurrentAreaCoordinate() - squadOne->GetCurrentAreaCoordinate();
	FIntPoint squadTwoAttackingDirection = squadOneAttackingDirection * -1;

	FIntPoint startingGridCoodinate = squadOne->GetCurrentAreaCoordinate() * 5;
	if (squadOneAttackingDirection.X == -1)
	{
		startingGridCoodinate.X += 4;
		startingGridCoodinate.Y += 4;
	}
	else if (squadOneAttackingDirection.Y == 1)
	{
		startingGridCoodinate.X += 4;
	}
	else if (squadOneAttackingDirection.Y == -1)
	{
		startingGridCoodinate.Y += 4;
	}

	bool bHasAnyUnitMoved = false;

	for (int lineIndex = 0; lineIndex < 5; lineIndex++) 
	{
		AEMPCombatUnit* mostAdvancedUnit = nullptr;
		bool bIsSpaceAvailableToAdvance = false;
		bool bIsSpaceContested = false;

		for (int i = 9; i >= 0; i--)
		{
			FIntPoint lineIndexOffset = GetPerpendicularOfIntPoint_CounterClockwise(squadOneAttackingDirection) * lineIndex;
			mostAdvancedUnit = squadOne->GetCombatUnitAtGridCoordinate((startingGridCoodinate + squadOneAttackingDirection * i) + lineIndexOffset);
			if (mostAdvancedUnit)
			{
				AEMPCombatUnit* potentialUnitInFront = squadTwo->GetCombatUnitAtGridCoordinate(mostAdvancedUnit->GetGridCoordinate() + squadOneAttackingDirection);
				AEMPCombatUnit* potentialUnitTwoSpacesInFront = squadTwo->GetCombatUnitAtGridCoordinate(mostAdvancedUnit->GetGridCoordinate() + squadOneAttackingDirection * 2);

				bIsSpaceAvailableToAdvance = potentialUnitInFront == nullptr;
				bIsSpaceContested = potentialUnitTwoSpacesInFront != nullptr;
				break;
			}
		}

		// Handle if no units in column
		bool bHasUnitInColumn = mostAdvancedUnit != nullptr;
		if (!bHasUnitInColumn)
		{
			// Let other team try to advance
			bHasAnyUnitMoved = squadTwo->AdvanceCombatUnits_SingleLine(squadTwoAttackingDirection, 4 - lineIndex, 1, !bHasAnyUnitMoved) || bHasAnyUnitMoved;
		}

		if (bIsSpaceAvailableToAdvance)
		{
			if (!bIsSpaceContested)
			{
				// Advance units in both squads
				if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("No space contested, both squads moving on line index %i"), lineIndex);
				bHasAnyUnitMoved = squadOne->AdvanceCombatUnits_SingleLine(squadOneAttackingDirection, lineIndex, 1, !bHasAnyUnitMoved) || bHasAnyUnitMoved;
				bHasAnyUnitMoved = squadTwo->AdvanceCombatUnits_SingleLine(squadTwoAttackingDirection, 4 - lineIndex, 1, !bHasAnyUnitMoved) || bHasAnyUnitMoved;
			}
			else if (bIsSpaceContested)
			{
				// If contested, roll for advancement (rule can change in future)
				float moveRoll = FMath::RandRange(0.0f, 1.0f);
				if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("Space contested, roll result: %f"), moveRoll);
				if (moveRoll > 0.5f)
				{
					// Move squad one
					bHasAnyUnitMoved = squadOne->AdvanceCombatUnits_SingleLine(squadOneAttackingDirection, lineIndex, 1, !bHasAnyUnitMoved) || bHasAnyUnitMoved;
				}
				else {
					// Move squad two
					bHasAnyUnitMoved = squadTwo->AdvanceCombatUnits_SingleLine(squadTwoAttackingDirection, 4 - lineIndex, 1, !bHasAnyUnitMoved) || bHasAnyUnitMoved;
				}
			}
		}
	}

	if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("Movement calculation finished, has any unit moved? %s"), bHasAnyUnitMoved ? TEXT("yes") : TEXT("no"));

	return bHasAnyUnitMoved;
}

void ABasicLevelGameModeEMP::SimulateSquadAttacks(AEMPSquad* attackingSquad, AEMPSquad* defendingSquad, FIntPoint attackingDirection)
{
	TArray<AEMPCombatUnit*> attackingSquadCombatUnits = attackingSquad->GetCombatUnitsInSquad();
	for (AEMPCombatUnit* unit : attackingSquadCombatUnits)
	{
		FIntPoint unitPosition = unit->GetGridCoordinate();

		// First check space in front of unit
		AEMPCombatUnit* potentialEnemy = defendingSquad->GetCombatUnitAtGridCoordinate(unitPosition + attackingDirection);

		// Check in front and over in both directions
		if (!potentialEnemy)
		{
			potentialEnemy = defendingSquad->GetCombatUnitAtGridCoordinate(unitPosition + attackingDirection + GetPerpendicularOfIntPoint_Clockwise(attackingDirection));

			if (!potentialEnemy)
			{
				potentialEnemy = defendingSquad->GetCombatUnitAtGridCoordinate(unitPosition + attackingDirection + GetPerpendicularOfIntPoint_CounterClockwise(attackingDirection));

				if (!potentialEnemy)
				{
					// This unit has no enemies to attack, check next unit
					continue;
				}
			}
		}
		// Enemy has been found, simulate attack (Roll chance. If hits, die)

		float hitRoll = FMath::RandRange(0.0f, 1.0f);
		if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("Unit (%s) rolling to hit (%s). Result: %f"), *unit->GetName(), *potentialEnemy->GetName(), hitRoll);
		if (hitRoll > .5f)
		{
			potentialEnemy->TakeCachedDamage(10);
		}
	}
}

FIntPoint ABasicLevelGameModeEMP::GetPerpendicularOfIntPoint_Clockwise(FIntPoint inIntPoint)
{
	return FIntPoint(inIntPoint.Y, -inIntPoint.X);
}

FIntPoint ABasicLevelGameModeEMP::GetPerpendicularOfIntPoint_CounterClockwise(FIntPoint inIntPoint)
{
	return FIntPoint(-inIntPoint.Y, inIntPoint.X);
}

void ABasicLevelGameModeEMP::InitializeCombat(AEMPSquad* squadOne, AEMPSquad* squadTwo)
{
	InCombatSquadOne = squadOne;
	InCombatSquadTwo = squadTwo;

	SetCurrentGameState(EBasicLevelGameStateEMP::GS_RESOLVING_COMBAT);
	CurrentCombatRound = 5;
	TryStartCombatRound();
}

void ABasicLevelGameModeEMP::TryStartCombatRound()
{
	bool bHasAnyUnitMoved = false;

	if (CurrentCombatRound >= 1) 
	{
		SimulateCombatDamage(InCombatSquadOne, InCombatSquadTwo);
		bHasAnyUnitMoved = UpdateCombatMovement(InCombatSquadOne, InCombatSquadTwo);

		CurrentCombatRound -= 1;

		// Normally, the next combat is started when unit animations are finished, but if there are no animations, start it here.
		if (!bHasAnyUnitMoved)
		{
			TryStartCombatRound();
		}
	}
	else
	{
		ResetSquadAfterCombat();
	}
}

void ABasicLevelGameModeEMP::ResetSquadAfterCombat()
{
	SetCurrentGameState(EBasicLevelGameStateEMP::GS_RESET_SQUAD_AFTER_COMBAT);
	FIntPoint squadOneAttackingDirection = InCombatSquadTwo->GetCurrentAreaCoordinate() - InCombatSquadOne->GetCurrentAreaCoordinate();

	bool bHasAnyUnitMoved = false;
	bHasAnyUnitMoved = InCombatSquadOne->ResetCombatUnitsAfterCombat(squadOneAttackingDirection, !bHasAnyUnitMoved) || bHasAnyUnitMoved;
	bHasAnyUnitMoved = InCombatSquadTwo->ResetCombatUnitsAfterCombat(squadOneAttackingDirection * -1, !bHasAnyUnitMoved) || bHasAnyUnitMoved;

	if (!bHasAnyUnitMoved)
	{
		EndCombat();
	}
}

void ABasicLevelGameModeEMP::EndCombat()
{
	InCombatSquadOne = nullptr;
	InCombatSquadTwo = nullptr;

	SetCurrentGameState(EBasicLevelGameStateEMP::GS_RESOLVING_ORDERS);
	ResolveCurrentCombatAction();
}
