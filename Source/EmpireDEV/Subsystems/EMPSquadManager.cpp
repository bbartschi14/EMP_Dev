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
}


void UEMPSquadManager::Deinitialize()
{
	Super::Deinitialize();
}

TArray<UEMPSquadData*> UEMPSquadManager::GetSquadData() const
{
	return Squads;
}

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


