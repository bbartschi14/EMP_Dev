// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../../../EMPUserWidget.h"
#include "EMPUWButtonBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class EMPIREDEV_API UEMPUWButtonBase : public UEMPUserWidget
{
	GENERATED_BODY()
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClick);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHover);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnhover);

	/** Called when the button is clicked interactively by the user */
	UPROPERTY(BlueprintAssignable, Category = "EMP UI Events")
		FOnClick OnClick;

	/** Called when the button is hovered interactively by the user */
	UPROPERTY(BlueprintAssignable, Category = "EMP UI Events")
		FOnHover OnHover;

	/** Called when the button is unhovered interactively by the user */
	UPROPERTY(BlueprintAssignable, Category = "EMP UI Events")
		FOnUnhover OnUnhover;

	UFUNCTION(BlueprintCallable)
		bool GetIsToggleOn() const;

	/** Controls the toggled state of this button */
	UFUNCTION()
		virtual void SetToggleOnOff(bool isOn);
protected:
	/** Use NativeConstruct to bind delegates to native button */
	void NativeConstruct() override;

	/** Can update the visual style when toggle on or off */
	UFUNCTION(BlueprintImplementableEvent)
		void HandleToggleOnOff(bool isOn);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* NativeButton = nullptr;

	/** When toggled on, this button should appear in a special "Selected" state */
	UPROPERTY(BlueprintReadOnly)
		bool bIsToggleOn;

private:
	UFUNCTION()
		void HandleOnNativeButtonClick();

	UFUNCTION()
		void HandleOnNativeButtonHovered();

	UFUNCTION()
		void HandleOnNativeButtonUnhovered();
};
