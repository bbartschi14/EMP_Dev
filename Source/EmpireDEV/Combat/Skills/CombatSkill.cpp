// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "CombatSkill.h"

void UEMPCombatSkill::SetOwningSquad(class UEMPSquadData* InOwningSquad)
{
	OwningSquad = InOwningSquad;
}

FName UEMPCombatSkill::GetSkillType() const
{
	return FName(TEXT("None"));
}
