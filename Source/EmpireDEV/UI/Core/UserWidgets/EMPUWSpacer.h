// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../../EMPUserWidget.h"
#include "EMPUWSpacer.generated.h"

class USpacer;

/**
 * Spacer with fixed size intervals for width and height.
 */
UCLASS()
class EMPIREDEV_API UEMPUWSpacer : public UEMPUserWidget
{
	GENERATED_BODY()
public:
	void SynchronizeProperties() override;

	/** */
	UFUNCTION(BlueprintCallable)
		void SetWidth(EEMPPaddingSize inWidthSize);

	/** */
	UFUNCTION(BlueprintCallable)
		void SetHeight(EEMPPaddingSize inHeightSize);


protected:
	// Style parameters
	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		EEMPPaddingSize WidthSize = EEMPPaddingSize::PS_SMALL;

	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		EEMPPaddingSize HeightSize = EEMPPaddingSize::PS_SMALL;

	// Bound widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		USpacer* NativeSpacer = nullptr;
};
