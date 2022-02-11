// Empire 2021. Ben Bartschi and Jacob Bartschi

#include "FEMPCombatUnitData.h"
#include "../Skills/CombatSkill.h"
#include "../Skills/EMPStatModifierCombatSkill.h"
#include "../../Game/GameInstanceBaseEMP.h"

UEMPCombatUnitData::UEMPCombatUnitData()
{
}

void UEMPCombatUnitData::InitializeDefaultSkills(const TArray<TSubclassOf<UEMPCombatSkill>>& InSkills)
{
	for (TSubclassOf<UEMPCombatSkill> skill : InSkills)
	{
		UEMPCombatSkill* newSkill = NewObject<UEMPCombatSkill>(this, skill);
		CombatSkills.Add(newSkill);
	}
}

void UEMPCombatUnitData::GetCombatSkillsOfType(TArray<UEMPCombatSkill*>& OutSkills, FName InType)
{
	for (auto skill : CombatSkills)
	{
		if (skill->GetSkillType().IsEqual(InType))
		{
			OutSkills.Add(skill);
		}
	}
}

void UEMPCombatUnitData::GetStatModifierCombatSkills(TArray<UEMPStatModifierCombatSkill*>& OutSkills)
{
	TArray<UEMPCombatSkill*> skills;
	GetCombatSkillsOfType(skills, FName(TEXT("StatModifier")));

	for (auto skill : skills)
	{
		OutSkills.Add(Cast<UEMPStatModifierCombatSkill>(skill));
	}
}

UEMPCombatUnitData* FEMPCombatUnitDataStruct::GetCombatUnitData(UGameInstanceBaseEMP* GameInstance) const
{
	UEMPCombatUnitData* combatUnitData = NewObject<UEMPCombatUnitData>();

	combatUnitData->CombatUnitName = CombatUnitName;
	combatUnitData->DesiredLocationX = DesiredLocationX;
	combatUnitData->DesiredLocationY = DesiredLocationY;

	// Combat

	combatUnitData->CombatLocation = CombatLocation;
	combatUnitData->CurrentHealth = CurrentHealth;

	// Classes and Ranks

	combatUnitData->CombatClass = CombatClass;
	combatUnitData->CombatClassRank = CombatClassRank;

	// Officer status

	combatUnitData->NCORank = NCORank;
	combatUnitData->OfficerRank = OfficerRank;

	// Stats
	combatUnitData->Health = Health;
	combatUnitData->Damage = Damage;
	combatUnitData->Armor = Armor;
	combatUnitData->Speed = Speed;
	combatUnitData->Range = Range;

	TArray<TSubclassOf<UEMPCombatSkill>> OutSkills;
	GameInstance->GetDefaultSkillsForClass(combatUnitData->CombatClass, OutSkills);
	combatUnitData->InitializeDefaultSkills(OutSkills);

	return combatUnitData;
}
