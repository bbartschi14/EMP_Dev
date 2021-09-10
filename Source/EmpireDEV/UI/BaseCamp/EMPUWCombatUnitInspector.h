// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../EMPUserWidget.h"
#include "EMPUWCombatUnitInspector.generated.h"

class UEMPUWNumberPropertyDisplay;
class UEMPUWText;
class UEMPCombatUnitData;
/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPUWCombatUnitInspector : public UEMPUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
		void SetCombatUnitData(class UEMPCombatUnitData* combatUnit);

protected:
	/** Reference to the combat unit that this inspector is displaying */
	UPROPERTY(Transient)
		UEMPCombatUnitData* CombatUnitRepresented;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UEMPUWText* CombatUnitNameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UEMPUWNumberPropertyDisplay* HealthDisplay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UEMPUWNumberPropertyDisplay* DamageDisplay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UEMPUWNumberPropertyDisplay* ArmorDisplay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UEMPUWNumberPropertyDisplay* SpeedDisplay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UEMPUWNumberPropertyDisplay* RangeDisplay;

private:
	/** Update all of the property displays to the data currently stored in CombatUnitRepresented */
	UFUNCTION()
		void SynchronizeDisplayToStoredData();
};
