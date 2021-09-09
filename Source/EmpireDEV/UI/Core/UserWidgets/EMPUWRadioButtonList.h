// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../../EMPUserWidget.h"
#include "EMPUWRadioButtonList.generated.h"

/**
 * Controls a list of radio buttons. Only one can be active at any given point in time.
 * Clicking a non-selected radio button will deselect the previously selected button.
 */
UCLASS()
class EMPIREDEV_API UEMPUWRadioButtonList : public UEMPUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioButtonSelected, class UEMPUWRadioButton*, radioButtonSelected);

	/** Called when the a radio button is selected. Broadcasts the new selected radio button */
	UPROPERTY(BlueprintAssignable, Category = "EMP UI Events")
		FOnRadioButtonSelected OnRadioButtonSelected;

	UFUNCTION(BlueprintCallable)
		void AddRadioButtonToList(class UEMPUWRadioButton* radioButtonWidget);

	UFUNCTION(BlueprintCallable)
		void ClearList();

	UFUNCTION(BlueprintCallable)
		void ClearSelected();

protected:
	/** Updates visuals of controlled radio buttons. Can override to implement additional logic. If overriden, must call Super to maintain core logic */
	UFUNCTION()
		virtual void HandleRadioButtonClicked(class UEMPUWRadioButton* radioButtonClicked);

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* RadioButtonsContainer;

	UPROPERTY(Transient)
		class UEMPUWRadioButton* SelectedRadioButton;

};
