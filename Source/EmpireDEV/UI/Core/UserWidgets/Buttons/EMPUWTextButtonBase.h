// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "EMPUWButtonBase.h"
#include "EMPUWTextButtonBase.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPUWTextButtonBase : public UEMPUWButtonBase
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
		void SetButtonSize(EEMPFontSize inSize);

protected:
	// Style parameters

	/** Inner button text */
	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		FText InnerText = FText::FromString("Button Text");

	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		TEnumAsByte<ETextJustify::Type> TextJustification = ETextJustify::Center;

	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		EEMPFontSize ButtonSize = EEMPFontSize::FS_SMALL;

	// Bound Widgets

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UEMPUWText* InnerTextLabel = nullptr;
};
