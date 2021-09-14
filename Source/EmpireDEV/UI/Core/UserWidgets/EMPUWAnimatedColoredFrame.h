// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../../EMPUserWidget.h"
#include "EMPUWAnimatedColoredFrame.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPUWAnimatedColoredFrame : public UEMPUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void NativeActivateBorder();
	UFUNCTION(BlueprintImplementableEvent)
		void ActivateBorder();

	UFUNCTION(BlueprintCallable)
		void NativeDeactivateBorder();
	UFUNCTION(BlueprintImplementableEvent)
		void DeactivateBorder();

	UFUNCTION(BlueprintCallable)
		void ActivateStaticBorder();
	UFUNCTION(BlueprintCallable)
		void DeactivateStaticBorder();

	UFUNCTION(BlueprintCallable)
		void SetStaticBorderOpacity(float opacity);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UEMPUWColoredFrame* StaticFrame = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UEMPUWColoredFrame* AnimatedFrame = nullptr;

	UPROPERTY(BlueprintReadOnly)
		bool bIsFrameActive;
};
