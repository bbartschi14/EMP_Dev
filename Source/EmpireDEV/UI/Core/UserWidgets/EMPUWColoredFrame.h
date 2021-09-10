// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../../EMPUserWidget.h"
#include "EMPUWColoredFrame.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPUWColoredFrame : public UEMPUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void ActivateBorder();

	UFUNCTION(BlueprintCallable)
		void DeactivateBorder();

	UFUNCTION(BlueprintCallable)
		void SetColor(FLinearColor inColor);

	UFUNCTION(BlueprintCallable)
		void SetBorderOffset(float inOffset);

	UFUNCTION(BlueprintCallable)
		void SetBorderThickness(float inThickness);
protected:

	/** Override to draw border lines (nice and antialiased)*/
	int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
	/** Controls visibility of border */
	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		bool bIsBorderActive = true;

	/** */
	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		bool bShowTop = true;

	/** */
	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		bool bShowBottom = true;

	/** */
	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		bool bShowRight = true;

	/** */
	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		bool bShowLeft = true;

	/** Positive offset increased the size of the border (in pixels) */
	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		float borderOffset = 0.0f;

	/** Pixel thickness of border */
	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		float borderThickness = 2.0f;

	/** Color of border */
	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		FLinearColor borderColor;

	/** Transparency control (widget render opacity doesn't work with line drawing) */
	UPROPERTY(EditAnywhere, Category = "EMP UI Style", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float borderOpacity = 1.0f;
};
