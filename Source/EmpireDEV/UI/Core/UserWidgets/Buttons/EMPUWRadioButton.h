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
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioButtonEvent, UEMPUWRadioButton*, radioButton);

	/** Called when the button is clicked interactively by the user */
	UPROPERTY(BlueprintAssignable, Category = "EMP UI Events")
		FOnRadioButtonEvent OnRadioButtonClicked;

	UPROPERTY(BlueprintAssignable, Category = "EMP UI Events")
		FOnRadioButtonEvent OnRadioButtonHovered;

	UPROPERTY(BlueprintAssignable, Category = "EMP UI Events")
		FOnRadioButtonEvent OnRadioButtonUnhovered;

	/** Controls the toggled state of the controlled button */
	UFUNCTION(BlueprintCallable)
		void SetToggleOnOff(bool isOn);

	UFUNCTION(BlueprintImplementableEvent)
		void HandleToggleOnOff(bool isOn);
protected:
	void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UEMPUWButtonBase* Button;

private:
	UFUNCTION()
		void HandleButtonClicked();

	UFUNCTION()
		void HandleButtonHovered();

	UFUNCTION()
		void HandleButtonUnhovered();
};
