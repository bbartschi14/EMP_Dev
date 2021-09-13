// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPBetweenGameMenuMode.h"
#include "../Level/EMPGridSquare.h"
#include "../Combat/Structs/FEMPSquadData.h"
#include "../Combat/Structs/FEMPCombatUnitData.h"
#include "../Combat/EMPCombatUnit.h"

void AEMPBetweenGameMenuMode::HandleCancelActionPressed()
{
	if (CurrentGameState == EBaseCampGameStateEMP::BC_SELECTING_SQUAD)
	{
		// Empty response
	}
	else if (CurrentGameState == EBaseCampGameStateEMP::BC_SELECTING_UNIT)
	{
		SetCurrentGameState(EBaseCampGameStateEMP::BC_SELECTING_SQUAD);
	}
	else if (CurrentGameState == EBaseCampGameStateEMP::BC_EDITING_UNIT)
	{
		SetCurrentGameState(EBaseCampGameStateEMP::BC_SELECTING_UNIT);
	}
	else if (CurrentGameState == EBaseCampGameStateEMP::BC_REARRANGING_SQUAD)
	{
		SetCurrentGameState(EBaseCampGameStateEMP::BC_SELECTING_UNIT);
	}
	else if (CurrentGameState == EBaseCampGameStateEMP::BC_MOVING_UNIT)
	{
		SetCurrentGameState(EBaseCampGameStateEMP::BC_REARRANGING_SQUAD);
	}
}

bool AEMPBetweenGameMenuMode::IsCombatUnitInSelectedSquad(UEMPCombatUnitData* combatUnit)
{
	if (SelectedSquad && SelectedSquad->CombatUnitsInSquad.Contains(combatUnit))
	{
		return true;
	}
	return false;
}

void AEMPBetweenGameMenuMode::BeginPlay()
{
	Super::BeginPlay();

	SpawnGrid();
}

void AEMPBetweenGameMenuMode::HandleGridSquareClicked(AEMPGridSquare* inGridSquare)
{
	AEMPCombatUnit* unit = GetCombatUnitAtLocation(inGridSquare->GetGridCoordinate());

	if (!IsGridConditionMetForGivenState(CurrentGameState, unit)) return;

	if (CurrentGameState == EBaseCampGameStateEMP::BC_SELECTING_SQUAD)
	{
		// Empty response
	}
	else if (CurrentGameState == EBaseCampGameStateEMP::BC_SELECTING_UNIT || CurrentGameState == EBaseCampGameStateEMP::BC_EDITING_UNIT || CurrentGameState == EBaseCampGameStateEMP::BC_REARRANGING_SQUAD)
	{
		SelectCombatUnit(unit->GetCombatUnitData());

		// "Rearranging squad" has the same behavior, except that it transitions to "moving unit" state
		if (CurrentGameState == EBaseCampGameStateEMP::BC_REARRANGING_SQUAD) 
		{
			SetCurrentGameState(EBaseCampGameStateEMP::BC_MOVING_UNIT);
		}
		else 
		{
			SetCurrentGameState(EBaseCampGameStateEMP::BC_EDITING_UNIT);
		}
	}
	else if (CurrentGameState == EBaseCampGameStateEMP::BC_MOVING_UNIT)
	{
		if (unit == nullptr || unit->GetCombatUnitData() != SelectedCombatUnit)
		{
			FIntPoint newLocation = inGridSquare->GetGridCoordinate();
			FIntPoint oldLocation = SelectedCombatUnit->GetDesiredLocation();

			AEMPCombatUnit* selectedUnitActor = GetCombatUnitAtLocation(oldLocation);

			if (unit)
			{
				// Swap other unit position
				unit->GetCombatUnitData()->SetDesiredLocation(oldLocation);
				unit->MoveToGridSquare(oldLocation, false);
			}

			// Move selected unit to position
			AEMPGridSquare* gridSquareToDeselect = GetGridSquareAtCoordinate(SelectedCombatUnit->GetDesiredLocation());
			if (gridSquareToDeselect) gridSquareToDeselect->SetSelected(false);
			SelectedCombatUnit->SetDesiredLocation(newLocation);
			if (selectedUnitActor)
			{
				selectedUnitActor->MoveToGridSquare(newLocation, false);
			}
		}
		
		ClearSelectedCombatUnit();

		SetCurrentGameState(EBaseCampGameStateEMP::BC_REARRANGING_SQUAD);
	}

}

void AEMPBetweenGameMenuMode::HandleSetGridHighlighted(AEMPGridSquare* inGridSquare, bool isHighlighted)
{
	AEMPCombatUnit* unit = GetCombatUnitAtLocation(inGridSquare->GetGridCoordinate());

	if (!IsGridConditionMetForGivenState(CurrentGameState, unit)) return;

	if (CurrentGameState == EBaseCampGameStateEMP::BC_SELECTING_SQUAD)
	{
		// Empty response
	}
	else if (CurrentGameState == EBaseCampGameStateEMP::BC_SELECTING_UNIT || 
			CurrentGameState == EBaseCampGameStateEMP::BC_EDITING_UNIT || 
			CurrentGameState == EBaseCampGameStateEMP::BC_REARRANGING_SQUAD ||
			CurrentGameState == EBaseCampGameStateEMP::BC_MOVING_UNIT)
	{
		inGridSquare->SetHighlighted(isHighlighted);
	}
}

bool AEMPBetweenGameMenuMode::IsGridConditionMetForGivenState(EBaseCampGameStateEMP givenState, AEMPCombatUnit* unit)
{
	if (givenState == EBaseCampGameStateEMP::BC_SELECTING_SQUAD)
	{
		return false;
	} 
	else if (givenState == EBaseCampGameStateEMP::BC_SELECTING_UNIT || givenState == EBaseCampGameStateEMP::BC_EDITING_UNIT || givenState == EBaseCampGameStateEMP::BC_REARRANGING_SQUAD)
	{
		return unit != nullptr;
	}
	else if (givenState == EBaseCampGameStateEMP::BC_MOVING_UNIT)
	{
		check(SelectedCombatUnit);
		return true;
	}
	return false;
}

void AEMPBetweenGameMenuMode::SelectSquad(UEMPSquadData* squadToLoad)
{
	check(squadToLoad);

	if (squadToLoad == SelectedSquad) return;

	if (SelectedSquad && SelectedCombatUnit && SelectedSquad->CombatUnitsInSquad.Contains(SelectedCombatUnit))
	{
		ClearSelectedCombatUnit();
	}

	// Update state and selected squad
	if (CurrentGameState != EBaseCampGameStateEMP::BC_SELECTING_UNIT)
	{
		SetCurrentGameState(EBaseCampGameStateEMP::BC_SELECTING_UNIT);
	}
	ClearSelectedSquad(false);

	SelectedSquad = squadToLoad;

	// Spawn Combat Units
	for (UEMPCombatUnitData* combatUnit : SelectedSquad->CombatUnitsInSquad)
	{
		AEMPCombatUnit* newCombatUnit = GetWorld()->SpawnActor<AEMPCombatUnit>(CombatUnitClass);
		newCombatUnit->InitializeCombatUnitData(combatUnit);
		CombatUnits.Add(newCombatUnit);
	}

	// Set Combat Unit Locations
	for (AEMPCombatUnit* unitToPlace : CombatUnits)
	{
		UEMPCombatUnitData* data = unitToPlace->GetCombatUnitData();

		FIntPoint desiredLocation = FIntPoint(data->DesiredLocationX, data->DesiredLocationY);

		check (IsLocationEmpty(desiredLocation)) // Sanity check, because the squad manager should already guarantee that desired positions are unique
		unitToPlace->InitializeToGridSquare(desiredLocation);
	}

	OnSquadSelected.Broadcast(SelectedSquad);
}

void AEMPBetweenGameMenuMode::SelectCombatUnit(UEMPCombatUnitData* combatUnitToLoad)
{
	ClearSelectedCombatUnit();
	SelectedCombatUnit = combatUnitToLoad;

	// Check if selected unit is unassigned
	if (SelectedSquad && SelectedSquad->CombatUnitsInSquad.Contains(SelectedCombatUnit))
	{
		AEMPGridSquare* potentialGridSquare = GetGridSquareAtCoordinate(SelectedCombatUnit->GetDesiredLocation());
		if (potentialGridSquare)
		{
			potentialGridSquare->SetSelected(true);
		}
	}
	else if (CurrentGameState == EBaseCampGameStateEMP::BC_REARRANGING_SQUAD || CurrentGameState == EBaseCampGameStateEMP::BC_MOVING_UNIT)
	{
		SetCurrentGameState(EBaseCampGameStateEMP::BC_SELECTING_UNIT);
	}

	OnCombatUnitSelected.Broadcast(SelectedCombatUnit);
}

void AEMPBetweenGameMenuMode::SetCurrentGameState(EBaseCampGameStateEMP newGameState)
{
	if (newGameState == EBaseCampGameStateEMP::BC_SELECTING_SQUAD)
	{
		ClearSelectedSquad(true);
		if (HoveredGridSquare) HoveredGridSquare->SetHighlighted(false);
	}
	else if (newGameState == EBaseCampGameStateEMP::BC_SELECTING_UNIT)
	{
		if (SelectedSquad && SelectedCombatUnit && SelectedSquad->CombatUnitsInSquad.Contains(SelectedCombatUnit))
		{
			ClearSelectedCombatUnit();
		}
	}
	else if (newGameState == EBaseCampGameStateEMP::BC_REARRANGING_SQUAD)
	{
		ClearSelectedCombatUnit();
		
		if (HoveredGridSquare) HoveredGridSquare->SetHighlighted(false);
	}


	CurrentGameState = newGameState;
	OnGameStateChanged.Broadcast(CurrentGameState);
}

void AEMPBetweenGameMenuMode::TryToEnterSquadRearranging()
{
	if (SelectedSquad)
	{
		if (SelectedCombatUnit && SelectedSquad->CombatUnitsInSquad.Contains(SelectedCombatUnit))
		{
			SetCurrentGameState(EBaseCampGameStateEMP::BC_MOVING_UNIT);
		}
		else
		{
			SetCurrentGameState(EBaseCampGameStateEMP::BC_REARRANGING_SQUAD);
		}
	}
}

void AEMPBetweenGameMenuMode::TryToExitSquadRearranging()
{
	SetCurrentGameState(EBaseCampGameStateEMP::BC_SELECTING_UNIT);
}

#pragma region Helpers
bool AEMPBetweenGameMenuMode::IsLocationEmpty(FIntPoint location) const
{
	return GetCombatUnitAtLocation(location) == nullptr;
}

AEMPCombatUnit* AEMPBetweenGameMenuMode::GetCombatUnitAtLocation(FIntPoint location) const
{
	for (AEMPCombatUnit* unitToCheck : CombatUnits)
	{
		if (unitToCheck->GetGridCoordinate() == location)
		{
			return unitToCheck;
		}
	}
	return nullptr;
}

void AEMPBetweenGameMenuMode::ClearSelectedCombatUnit()
{
	if (SelectedCombatUnit)
	{
		AEMPGridSquare* gridSquare = GetGridSquareAtCoordinate(SelectedCombatUnit->GetDesiredLocation());
		gridSquare->SetSelected(false);

		SelectedCombatUnit = nullptr;

		OnCombatUnitDeselected.Broadcast();
	}
}
void AEMPBetweenGameMenuMode::ClearSelectedSquad(bool bShouldBroadcast)
{
	if (SelectedSquad)
	{
		// Clear previous units
		for (AEMPCombatUnit* unitToDestroy : CombatUnits)
		{
			unitToDestroy->Destroy();
		}
		CombatUnits.Empty();

		SelectedSquad = nullptr;

		if (bShouldBroadcast)
		{
			OnSquadDeselected.Broadcast();
		}
	}
}
#pragma endregion Helpers

