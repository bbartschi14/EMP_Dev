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
	return ActiveOfficer;
}

void UEMPSquadData::SetSquadOfficer(UEMPCombatUnitData* NewOfficer)
{
	if (CombatUnitsInSquad.Contains(NewOfficer))
	{
		ActiveOfficer = NewOfficer;
	}
	else if (NewOfficer == nullptr)
	{
		ActiveOfficer = nullptr;
	}
	RefreshCombatSkills();
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
	int32 distance = FMath::Abs(GetCombatAreaLocation().X - areaCoordinate.X) + FMath::Abs(GetCombatAreaLocation().Y - areaCoordinate.Y);
	return distance == 1;
}

void UEMPSquadData::HandleCombatUnitDied(UEMPCombatUnitData* deadCombatUnit)
{
	RemoveCombatUnit(deadCombatUnit);
}

void UEMPSquadData::RemoveCombatUnit(UEMPCombatUnitData* CombatUnitToRemove)
{
	CombatUnitsInSquad.Remove(CombatUnitToRemove);
	if (GetSquadOfficer() == CombatUnitToRemove)
	{
		SetSquadOfficer(nullptr);
	}
	RefreshCombatSkills();
}

void UEMPSquadData::AddCombatUnit(UEMPCombatUnitData* CombatUnitToAdd)
{
	CombatUnitsInSquad.Add(CombatUnitToAdd);
	CombatUnitToAdd->OwningSquad = this;

	// If we add an officer and the squad doesn't already have an officer, we assign this one for convenience
	if (GetSquadOfficer() == nullptr && CombatUnitToAdd->OfficerRank != EEMPOfficerRank::OR_NONE)
	{
		SetSquadOfficer(CombatUnitToAdd);
	}

	RefreshCombatSkills();
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

void UEMPSquadData::RefreshCombatSkills()
{
	CombatSkills.Empty();

	// Add Squad combat skills
	for (TSubclassOf<UEMPCombatSkill> skillClass : GameInstanceRef->SquadCombatSkills->CombatSkills)
	{
		UEMPCombatSkill* newSkill = NewObject<UEMPCombatSkill>(this, skillClass);
		if (newSkill->AreSkillRequirementsMet(this, nullptr))
		{
			newSkill->SetOwningSquad(this);
			CombatSkills.Add(newSkill);
		}
	}

	// Add officer combat skills
	auto officer = GetSquadOfficer();
	if (officer != nullptr)
	{
		for (TSubclassOf<UEMPCombatSkill> skillClass : GameInstanceRef->OfficerCombatSkills->CombatSkills)
		{
			UEMPCombatSkill* newSkill = NewObject<UEMPCombatSkill>(this, skillClass);
			if (newSkill->AreSkillRequirementsMet(this, officer))
			{
				CombatSkills.Add(newSkill);
			}
		}
	}

	OnSquadSkillsRefreshed.Broadcast();
}

void UEMPSquadData::GetCombatSkills(TArray<UEMPCombatSkill*>& OutSkills) const
{
	for (auto skill : CombatSkills)
	{
		OutSkills.Add(skill);
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

void UEMPSquadData::SetCombatAreaLocation(FIntPoint val)
{
	CombatAreaLocation = val;
	OnSquadMovedToAreaLocation.Broadcast();
}

ESquadStrategyEMP UEMPSquadData::GetSquadStrategy() const
{
	return SquadStrategy;
}

void UEMPSquadData::SetSquadStrategy(ESquadStrategyEMP NewStrategy)
{
	SquadStrategy = NewStrategy;
	OnSquadStrategyChanged.Broadcast();
}


UEMPSquadData* FEMPSquadDataStruct::GetSquadData(UGameInstanceBaseEMP* GameInstance) const
{
	UEMPSquadData* squadData = NewObject<UEMPSquadData>();
	squadData->GameInstanceRef = GameInstance;
	squadData->SquadName = SquadName;
	squadData->SetCombatAreaLocation(CombatAreaLocation);
	squadData->CombatDirection = CombatDirection;
	squadData->CurrentSquadState = CurrentSquadState;
	squadData->SetSquadStrategy(SquadStrategy);
	for (FEMPCombatUnitDataStruct combatUnit : CombatUnitsInSquad)
	{
		UEMPCombatUnitData* combatUnitData = combatUnit.GetCombatUnitData(GameInstance, squadData);
		combatUnitData->OwningSquad = squadData;
		squadData->CombatUnitsInSquad.Add(combatUnitData);

		// TODO: Change from simply setting the last officer
		if (combatUnitData->OfficerRank != EEMPOfficerRank::OR_NONE)
		{
			squadData->SetSquadOfficer(combatUnitData);
		}
	}

	squadData->RefreshCombatSkills();

	return squadData;
}
