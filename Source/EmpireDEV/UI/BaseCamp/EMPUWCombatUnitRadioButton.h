// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../Core/UserWidgets/Buttons/EMPUWRadioButton.h"
#include "../../Combat/Structs/FEMPCombatUnitData.h"
#include "EMPUWCombatUnitRadioButton.generated.h"

/**
 * Used as a single radio button representing a combat unit in the squad editor
 */
UCLASS()
class EMPIREDEV_API UEMPUWCombatUnitRadioButton : public UEMPUWRadioButton
{
	GENERATED_BODY()
public:
	/** Assign the unit that this button should represent */
	UFUNCTION()
		void SetCombatUnitData(UEMPCombatUnitData* inCombatUnit);

	/** Get the unit that this button represents */
	UFUNCTION()
		UEMPCombatUnitData* GetCombatUnitData() const;

protected:
	/** Reference to the unit that this button represents */
	UPROPERTY(Transient)
		UEMPCombatUnitData* CombatUnitRepresented;
};
