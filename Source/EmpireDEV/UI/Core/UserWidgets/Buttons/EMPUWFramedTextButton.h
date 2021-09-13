// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "EMPUWTextButtonBase.h"
#include "EMPUWFramedTextButton.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPUWFramedTextButton : public UEMPUWTextButtonBase
{
	GENERATED_BODY()

public:

	virtual void SynchronizeProperties() override;

	/** Sets the proper button color from the style sheet */
	UFUNCTION(BlueprintCallable)
		void SetButtonColor(FLinearColor inColor);

	/** Toggles visibility of icon image */
	UFUNCTION(BlueprintCallable)
		void SetUseOptionalImage(bool inValue);

	/** Sets the icon image */
	UFUNCTION(BlueprintCallable)
		void SetOptionalImageResource(UObject* inObject);

protected:
	// Style parameters

	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		FLinearColor ButtonColor;

	UPROPERTY(EditAnywhere, Category = "EMP UI Style", meta = (InlineEditConditionToggle))
		bool bUseOptionalIcon;

	UPROPERTY(EditAnywhere, Category = "EMP UI Style", meta = (editcondition = "bUseOptionalIcon"))
		UObject* OptionalIconImage;

	// Bound Widgets

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UEMPUWAnimatedColoredFrame* ColoredFrame = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* IconImage = nullptr;
};
