// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWCombatUnitRadioButton.h"
#include "../Core/UserWidgets/Buttons/EMPUWTextButtonBase.h"

void UEMPUWCombatUnitRadioButton::SetCombatUnitData(UEMPCombatUnitData* inCombatUnit)
{
	CombatUnitRepresented = inCombatUnit;
	SetText(FText::FromString(CombatUnitRepresented->CombatUnitName));
}

UEMPCombatUnitData* UEMPUWCombatUnitRadioButton::GetCombatUnitData() const
{
	return CombatUnitRepresented;
}
