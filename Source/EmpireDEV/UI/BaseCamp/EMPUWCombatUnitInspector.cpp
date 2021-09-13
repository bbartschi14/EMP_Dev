// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWCombatUnitInspector.h"
#include "../../Combat/Structs/FEMPCombatUnitData.h"
#include "../Core/UserWidgets/EMPUWText.h"
#include "../Core/UserWidgets/EMPUWNumberPropertyDisplay.h"

void UEMPUWCombatUnitInspector::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}


void UEMPUWCombatUnitInspector::SetCombatUnitData(UEMPCombatUnitData* combatUnit)
{
	CombatUnitRepresented = combatUnit;

	SynchronizeDisplayToStoredData();
}

void UEMPUWCombatUnitInspector::SynchronizeDisplayToStoredData()
{
	CombatUnitNameText->SetText(FText::FromString(CombatUnitRepresented->CombatUnitName));

	HealthDisplay->SetNumber(CombatUnitRepresented->Health);
	DamageDisplay->SetNumber(CombatUnitRepresented->Damage);
	ArmorDisplay->SetNumber(CombatUnitRepresented->Armor);
	SpeedDisplay->SetNumber(CombatUnitRepresented->Speed);
	RangeDisplay->SetNumber(CombatUnitRepresented->Range);
}