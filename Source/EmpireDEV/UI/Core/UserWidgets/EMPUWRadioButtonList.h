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
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRadioButtonSelected, class UEMPUWRadioButton*, radioButtonSelected, int32, indexSelected);

	/** Called when the a radio button is selected. Broadcasts the new selected radio button */
	UPROPERTY(BlueprintAssignable, Category = "EMP UI Events")
		FOnRadioButtonSelected OnRadioButtonSelected;

	UFUNCTION(BlueprintCallable)
		void AddRadioButtonToList(class UEMPUWRadioButton* radioButtonWidget);

	UFUNCTION(BlueprintCallable)
		void ClearList();

	UFUNCTION(BlueprintCallable)
		void ClearSelected();

	UFUNCTION(BlueprintCallable)
		void SetSelectedIndex(int32 indexToSelect);

	UFUNCTION(BlueprintCallable)
		TArray<class UEMPUWRadioButton*> GetRadioButtons() const;

	UFUNCTION(BlueprintCallable)
		class UEMPUWRadioButton* GetSelectedRadioButton();

	UFUNCTION(BlueprintCallable)
		void RemoveRadioButton(class UEMPUWRadioButton* radioButtonToRemove);

protected:
	/** Updates visuals of controlled radio buttons. Can override to implement additional logic. If overriden, must call Super to maintain core logic */
	UFUNCTION()
		virtual void HandleRadioButtonClicked(class UEMPUWRadioButton* radioButtonClicked);

	/** When true, clicking a radio button while cause it to toggle on. When false, it will wait for an external event to toggle on */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bTogglesSelf;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* RadioButtonsContainer;

	UPROPERTY(Transient)
		class UEMPUWRadioButton* SelectedRadioButton;

private:
	UFUNCTION()
		int32 GetIndexOfButton(class UEMPUWRadioButton* buttonToGetIndex);

	UFUNCTION()
		void SetAsSelected(class UEMPUWRadioButton* buttonToSetSelected);

};
