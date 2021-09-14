// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../../../EMPUserWidget.h"
#include "EMPUWRadioButton.generated.h"

/**
 *
 */
UCLASS()
class EMPIREDEV_API UEMPUWRadioButton : public UEMPUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioButtonClicked, UEMPUWRadioButton*, radioButton);

	/** Called when the button is clicked interactively by the user */
	UPROPERTY(BlueprintAssignable, Category = "EMP UI Events")
		FOnRadioButtonClicked OnRadioButtonClicked;

	/** Controls the toggled state of the controlled button */
	UFUNCTION()
		void SetToggleOnOff(bool isOn);

	UFUNCTION(BlueprintCallable)
		void SetText(FText inText);
protected:
	void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UEMPUWTextButtonBase* TextButton;

private:
	UFUNCTION()
		void HandleTextButtonClicked();
};
