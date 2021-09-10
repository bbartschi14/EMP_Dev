// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "EMPUWButtonBase.h"
#include "EMPUWTextButtonTemplate.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPUWTextButtonTemplate : public UEMPUWButtonBase
{
	GENERATED_BODY()
public:

	virtual void SynchronizeProperties() override;

	/** Set the inner button text */
	UFUNCTION(BlueprintCallable)
		void SetButtonText(FText inText);

	/** Set the inner text justification*/
	UFUNCTION(BlueprintCallable)
		void SetTextJustification(TEnumAsByte<ETextJustify::Type> inJustification);

	/** Set the button size by inner font size */
	UFUNCTION(BlueprintCallable)
		void SetButtonSize(int32 inSize);

	/** Sets the proper button color from the style sheet */
	UFUNCTION(BlueprintCallable)
		void SetButtonColor(FLinearColor inColor);

	/** Toggles visibility of icon image */
	UFUNCTION(BlueprintCallable)
		void SetUseOptionalImage(bool inValue);

	/** Sets the icon image */
	UFUNCTION(BlueprintCallable)
		void SetOptionalImageResource(UObject* inObject);

	/** Keeps the border active */
	UFUNCTION(BlueprintCallable)
		void SetStayActive(bool inValue);

protected:
	// Style parameters

	/** Inner button text */
	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		FText InnerText = FText::FromString("Button Text");

	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		TEnumAsByte<ETextJustify::Type> TextJustification = ETextJustify::Center;

	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		int32 ButtonSize = 14;

	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		FLinearColor ButtonColor;

	UPROPERTY(EditAnywhere, Category = "EMP UI Style", meta = (InlineEditConditionToggle))
		bool bUseOptionalIcon;

	UPROPERTY(EditAnywhere, Category = "EMP UI Style", meta = (editcondition = "bUseOptionalIcon"))
		UObject* OptionalIconImage;

	/** Set the colored frame state to always show */
	UPROPERTY(EditAnywhere, Category = "BYC UI Style")
		bool bIsStayActive;

	// Bound Widgets

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UEMPUWText* InnerTextLabel = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UEMPUWAnimatedColoredFrame* ColoredFrame = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* IconImage = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UOverlay* PaddedOverlay = nullptr;
};
