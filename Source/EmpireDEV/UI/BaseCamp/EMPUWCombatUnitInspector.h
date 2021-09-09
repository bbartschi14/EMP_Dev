// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../EMPUserWidget.h"
#include "EMPUWCombatUnitInspector.generated.h"

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
		class UEMPCombatUnitData* CombatUnitRepresented;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* NativeText;

};
