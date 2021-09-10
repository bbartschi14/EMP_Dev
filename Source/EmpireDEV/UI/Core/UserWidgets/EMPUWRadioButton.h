// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../../EMPUserWidget.h"
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

	/** Called when the radio button is clicked on */
	UPROPERTY(BlueprintAssignable, Category = "EMP UI Events")
		FOnRadioButtonClicked OnRadioButtonClicked;

	/** Controls the toggled state of this radio button */
	UFUNCTION()
		void SetToggleOnOff(bool isOn);

protected:
	/** Use NativeConstruct to bind delegates to native button */
	void NativeConstruct() override;

	UFUNCTION()
		void HandleMainButtonClicked();

	/** Can update the visual style when toggle on or off */
	UFUNCTION(BlueprintNativeEvent)
		void HandleToggleOnOff(bool isOn);

	/** Called by HandleToggleOnOff. Can override in C++ for custom subclass behavior */
	UFUNCTION()
		virtual void HandleToggleOnOffInternal(bool isOn);

	/** When toggled on, this button should appear in a special "Selected" state */
	UPROPERTY(BlueprintReadOnly)
		bool bIsToggleOn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UEMPUWButtonBase* MainButton;
};
