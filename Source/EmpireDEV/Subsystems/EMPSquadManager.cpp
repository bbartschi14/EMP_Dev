// Empire 2021. Ben Bartschi and Jacob Bartschi

#include "EMPSquadManager.h"
#include "../Game/GameInstanceBaseEMP.h"
#include "../Combat/Structs/EMPSquadsDatabase.h"
#include "../Combat/Structs/FEMPSquadData.h"

void UEMPSquadManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UGameInstanceBaseEMP* gameInstance = Cast<UGameInstanceBaseEMP>(GetWorld()->GetGameInstance());
	
	for (FEMPSquadDataStruct squadData : gameInstance->TestOnlySquadsDatabase->Squads)
	{
		UEMPSquadData* dataToStore = squadData.GetSquadData();
		bool bSquadValidated = ValidateSquad(dataToStore);
		if (bSquadValidated) Squads.Add(dataToStore);
	}

	for (FEMPCombatUnitDataStruct unitData : gameInstance->TestOnlySquadsDatabase->UnassignedCombatUnits)
	{
		UnassignedCombatUnits.Add(unitData.GetCombatUnitData());
	}
}

void UEMPSquadManager::Deinitialize()
{
	Super::Deinitialize();
}

TArray<UEMPSquadData*> UEMPSquadManager::GetSquadData() const
{
	return Squads;
}

TArray<class UEMPCombatUnitData*> UEMPSquadManager::GetUnassignedCombatUnitData() const
{
	return UnassignedCombatUnits;
}

bool UEMPSquadManager::RemoveCombatUnitFromSquad(UEMPCombatUnitData* combatUnit, class UEMPSquadData* squad)
{
	if (squad->CombatUnitsInSquad.Contains(combatUnit))
	{
		squad->CombatUnitsInSquad.Remove(combatUnit);
		UnassignedCombatUnits.Add(combatUnit);
		return true;
	}

	return false;
}

bool UEMPSquadManager::AssignCombatUnitToSquad(UEMPCombatUnitData* combatUnit, UEMPSquadData* squad)
{
	if (UnassignedCombatUnits.Contains(combatUnit) && squad->CombatUnitsInSquad.Num() < MaxSquadSize)
	{
		squad->CombatUnitsInSquad.Add(combatUnit);
		if (!DoesCombatUnitHaveUniqueLocationInSquad(squad, combatUnit))
		{
			FIntPoint newLocation = FindUniqueLocationInSquad(squad);
			combatUnit->SetDesiredLocation(newLocation);
		}
		return true;
	}

	return false;
}

void UEMPSquadManager::CreateNewSquad()
{
	UEMPSquadData* newSquad = NewObject<UEMPSquadData>();
	newSquad->SquadName = FString::Printf(TEXT("New Squad %i"), Squads.Num());
	Squads.Add(newSquad);
	OnSquadCreated.Broadcast(newSquad);
}

bool UEMPSquadManager::DissolveSquad(UEMPSquadData* squadToDissolve)
{
	for (UEMPSquadData* squad : Squads)
	{
		if (squad == squadToDissolve)
		{
			// Remove all the combat units to maintain as a part of the army
			TArray < UEMPCombatUnitData*> unitsToRemove;
			for (UEMPCombatUnitData* combatUnit : squad->CombatUnitsInSquad)
			{
				unitsToRemove.Add(combatUnit);
			}
			for (UEMPCombatUnitData* unitToRemove : unitsToRemove)
			{
				RemoveCombatUnitFromSquad(unitToRemove, squad);
			}
			// Remove squad from squads array then broadcast
			Squads.Remove(squad);
			OnSquadDissolved.Broadcast(squad);

			return true;
		}
	}
	return false; // Squad did not exist in this manager
}

#pragma region Helpers
bool UEMPSquadManager::ValidateSquad(UEMPSquadData* squadToValidate)
{
	// Check and update combat unit locations
	for (UEMPCombatUnitData* combatUnitData : squadToValidate->CombatUnitsInSquad)
	{
		if (!DoesCombatUnitHaveUniqueLocationInSquad(squadToValidate, combatUnitData))
		{
			FIntPoint newLocation = FindUniqueLocationInSquad(squadToValidate);
			if (newLocation == FIntPoint(-1, -1)) return false; // Failed to find new location

			combatUnitData->DesiredLocationX = newLocation.X;
			combatUnitData->DesiredLocationY = newLocation.Y;
		}
	}

	return true;
}

bool UEMPSquadManager::DoesCombatUnitHaveUniqueLocationInSquad(UEMPSquadData* squad, UEMPCombatUnitData* originalUnit)
{
	for (UEMPCombatUnitData* combatUnitData : squad->CombatUnitsInSquad)
	{
		if (originalUnit == combatUnitData) continue;

		if (combatUnitData->DesiredLocationX == originalUnit->DesiredLocationX && combatUnitData->DesiredLocationY == originalUnit->DesiredLocationY)
		{
			return false;
		}
	}
	return true;
}

FIntPoint UEMPSquadManager::FindUniqueLocationInSquad(class UEMPSquadData* squad)
{
	for (int32 row = 0; row < 5; row++)
	{
		for (int32 col = 0; col < 5; col++)
		{
			bool bIsLocationAvailable = true;
			for (UEMPCombatUnitData* data : squad->CombatUnitsInSquad)
			{
				if (data && data->DesiredLocationX == row && data->DesiredLocationY == col)
				{
					bIsLocationAvailable = false;
				}
			}

			if (bIsLocationAvailable)
			{
				return FIntPoint(row, col);
			}
		}
	}

	// If no positions available, return a failure point
	return FIntPoint(-1, -1);
}
#pragma endregion Helpers
