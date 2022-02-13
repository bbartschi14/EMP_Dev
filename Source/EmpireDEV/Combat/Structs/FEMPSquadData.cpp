// Empire 2021. Ben Bartschi and Jacob Bartschi

#include "FEMPSquadData.h"
#include "../Skills/CombatSkill.h"
#include "../Skills/EMPCombatActionSkill.h"

UEMPCombatUnitData* UEMPSquadData::GetCombatUnitAtDesiredLocation(FIntPoint desiredLocation) const
{
	for (UEMPCombatUnitData* combatUnit : CombatUnitsInSquad)
	{
		if (combatUnit->GetDesiredLocation() == desiredLocation)
		{
			return combatUnit;
		}
	}
	return nullptr;
}

UEMPCombatUnitData* UEMPSquadData::GetCombatUnitAtCombatLocation(FIntPoint combatLocation) const
{
	for (UEMPCombatUnitData* combatUnit : CombatUnitsInSquad)
	{
		if (combatUnit->CombatLocation == combatLocation)
		{
			return combatUnit;
		}
	}
	return nullptr;
}

UEMPCombatUnitData* UEMPSquadData::GetSquadOfficer() const
{
	for (UEMPCombatUnitData* combatUnit : CombatUnitsInSquad)
	{
		if (combatUnit->OfficerRank != EEMPOfficerRank::OR_NONE)
		{
			return combatUnit;
		}
	}
	return nullptr;
}

int32 UEMPSquadData::GetSquadMorale() const
{
	int32 morale = 0;
	for (UEMPCombatUnitData* combatUnit : CombatUnitsInSquad)
	{
		morale += 5; // 5 base morale per unit 
		if (combatUnit->OfficerRank != EEMPOfficerRank::OR_NONE)
		{
			morale += 75; // 75 additional morale per officer
		}
		else if (combatUnit->NCORank != EEMPNCORank::NCO_NONE)
		{
			morale += 15; // 15 additional morale per NCO
		}
	}
	return morale;
}

TMap<EEMPCombatClass, int32> UEMPSquadData::GetSquadComposition() const
{
	TMap<EEMPCombatClass, int32> composition;
	for (UEMPCombatUnitData* combatUnit : CombatUnitsInSquad)
	{
		int32 count = composition.FindOrAdd(combatUnit->CombatClass);
		composition[combatUnit->CombatClass] = count + 1;
	}
	return composition;
}

bool UEMPSquadData::CanMoveToAreaCoordinate(FIntPoint areaCoordinate) const
{
	int32 distance = FMath::Abs(CombatAreaLocation.X - areaCoordinate.X) + FMath::Abs(CombatAreaLocation.Y - areaCoordinate.Y);
	return distance == 1;
}

void UEMPSquadData::HandleCombatUnitDied(UEMPCombatUnitData* deadCombatUnit)
{
	CombatUnitsInSquad.Remove(deadCombatUnit);
}

void UEMPSquadData::GetCombatActionSkills(TArray<UEMPCombatActionSkill*>& OutSkills) const
{
	for (auto skill : CombatSkills)
	{
		UEMPCombatActionSkill* potentialAction = Cast<UEMPCombatActionSkill>(skill);
		if (potentialAction)
		{
			OutSkills.Add(potentialAction);
		}
	}
}

void UEMPSquadData::GetCombatUnitsOfClass(EEMPCombatClass InClass, TArray<class UEMPCombatUnitData*>& OutUnits) const
{
	for (auto unit : CombatUnitsInSquad)
	{
		if (unit->CombatClass == InClass)
		{
			OutUnits.Add(unit);
		}
	}
}

int32 UEMPSquadData::GetNumberOfCombatUnitsOfClass(EEMPCombatClass InClass) const
{
	int count = 0;
	for (auto unit : CombatUnitsInSquad)
	{
		if (unit->CombatClass == InClass)
		{
			count++;
		}
	}
	return count;
}

UEMPSquadData* FEMPSquadDataStruct::GetSquadData(UGameInstanceBaseEMP* GameInstance) const
{
	UEMPSquadData* squadData = NewObject<UEMPSquadData>();
	squadData->SquadName = SquadName;
	squadData->CombatAreaLocation = CombatAreaLocation;
	squadData->CombatDirection = CombatDirection;
	squadData->CurrentSquadState = CurrentSquadState;
	for (FEMPCombatUnitDataStruct combatUnit : CombatUnitsInSquad)
	{
		UEMPCombatUnitData* combatUnitData = combatUnit.GetCombatUnitData(GameInstance);
		combatUnitData->OwningSquad = squadData;
		squadData->CombatUnitsInSquad.Add(combatUnitData);
	}

	// Add Squad combat skills
	for (TSubclassOf<UEMPCombatSkill> skillClass : GameInstance->SquadCombatSkills->CombatSkills)
	{
		UEMPCombatSkill* newSkill = NewObject<UEMPCombatSkill>(squadData, skillClass);
		if (newSkill->AreSkillRequirementsMet(squadData, nullptr))
		{
			newSkill->SetOwningSquad(squadData);
			squadData->CombatSkills.Add(newSkill);
		}
	}

	return squadData;
}
