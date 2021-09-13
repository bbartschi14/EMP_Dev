// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../../EMPUserWidget.h"
#include "EMPUWText.generated.h"

/**
 * Used for text display.
 */
UCLASS()
class EMPIREDEV_API UEMPUWText : public UEMPUserWidget
{
	GENERATED_BODY()

public:
	void SynchronizeProperties() override;

	UFUNCTION(BlueprintCallable)
		void SetText(const FText& InText);

	UFUNCTION(BlueprintCallable)
		void SetFontSize(EEMPFontSize InFontSize);

	UFUNCTION(BlueprintCallable)
		void SetJustification(TEnumAsByte<ETextJustify::Type> InJustification);

	UFUNCTION(BlueprintCallable)
		const FText& GetText() const;

protected:
	// Style parameters
	UPROPERTY(EditAnywhere, Category = "BYC UI Style")
		EEMPFontSize FontSize = EEMPFontSize::FS_SMALL;

	UPROPERTY(EditAnywhere, Category = "BYC UI Style")
		TEnumAsByte<ETextJustify::Type> Justification;

	// Input parameters
	UPROPERTY(EditAnywhere, Category = "BYC UI Input")
		FText Text;

	// Bound widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* NativeText;
};
