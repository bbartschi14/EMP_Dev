// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../EMPUserWidget.h"
#include "../../Game/EMPBetweenGameMenuMode.h"
#include "EMPUWSquadEditor.generated.h"

/**
 * Panel used in the base camp to handle squad management
 */
UCLASS()
class EMPIREDEV_API UEMPUWSquadEditor : public UEMPUserWidget
{
	GENERATED_BODY()

protected:
	/** Use NativeOnInitialized to try to load up squad data */
	void NativeOnInitialized() override;

	void NativeConstruct() override;

	/** Clears the radio button list and reconstructs based on SquadManager subsystem data */
	UFUNCTION()
		void SynchSquadData();

	UPROPERTY(meta = (BindWidget))
		class UEMPUWRadioButtonList* SquadRadioButtonList;

	UPROPERTY(meta = (BindWidget))
		class UEMPUWSquadInspector* SquadInspector;

	UPROPERTY(meta = (BindWidget))
		class UEMPUWCombatUnitInspector* CombatUnitInspector;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UEMPUWSquadRadioButton> SquadRadioButtonClass;

protected:

	UFUNCTION()
		void HandleRadioButtonSelected(class UEMPUWRadioButton* radioButton);

	UFUNCTION()
		void HandleCombatUnitSelected(class UEMPCombatUnitData* combatUnitData);

	UFUNCTION()
		void HandleGameStateChanged(EBaseCampGameStateEMP newState);

	UFUNCTION()
		void HandleSquadDeselected();
};
