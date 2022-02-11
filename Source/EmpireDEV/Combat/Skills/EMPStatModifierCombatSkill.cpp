// Empire 2021. Ben Bartschi and Jacob Bartschi

#include "EMPStatModifierCombatSkill.h"

void UEMPStatModifierCombatSkill::SetData(FEMPStatModifierCombatSkillData InData)
{
	Data = InData;
}

float UEMPStatModifierCombatSkill::ModifyHitChance_Implementation(float OriginalHitChance)
{
	return OriginalHitChance;
}

int UEMPStatModifierCombatSkill::ModifyDamage_Implementation(int OriginalDamage)
{
	return OriginalDamage;
}

int UEMPStatModifierCombatSkill::ModifyArmor_Implementation(int OriginalArmor)
{
	return OriginalArmor;
}

FName UEMPStatModifierCombatSkill::GetSkillType() const
{
	return FName(TEXT("StatModifier"));
}
