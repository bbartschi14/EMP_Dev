// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWCombatUnitInspector.h"
#include "../../Combat/Structs/FEMPCombatUnitData.h"
#include "Components/TextBlock.h"

void UEMPUWCombatUnitInspector::SetCombatUnitData(UEMPCombatUnitData* combatUnit)
{
	NativeText->SetText(FText::FromString(combatUnit->CombatUnitName));
}
