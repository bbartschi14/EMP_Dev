// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWColoredFrame.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"

void UEMPUWColoredFrame::ActivateBorder()
{
	bIsBorderActive = true;
}

void UEMPUWColoredFrame::DeactivateBorder()
{
	bIsBorderActive = false;
}

void UEMPUWColoredFrame::SetColor(FLinearColor inColor)
{
	borderColor = inColor;
}

void UEMPUWColoredFrame::SetBorderOffset(float inOffset)
{
	borderOffset = inOffset;
}

void UEMPUWColoredFrame::SetBorderThickness(float inThickness)
{
	borderThickness = inThickness;
}

void UEMPUWColoredFrame::SetBorderOpacity(float inOpacity)
{
	borderOpacity = inOpacity;
}

int32 UEMPUWColoredFrame::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	//const bool bHasGameViewport = GEngine && GEngine->GameViewport;
	if (bIsBorderActive)
	{
		//FVector2D viewportSize;
		// as this function returns through the parameter, we just need to call it by passing in our FVector2D variable
		//GEngine->GameViewport->GetViewportSize(viewportSize);

		// we need to floor the float values of the viewport size so we can pass those into the GetDPIScaleBasedOnSize function
		//int32 X = FGenericPlatformMath::FloorToInt(viewportSize.X);
		//int32 Y = FGenericPlatformMath::FloorToInt(viewportSize.Y);

		// the GetDPIScaleBasedOnSize function takes an FIntPoint, so we construct one out of the floored floats of the viewport
		// the fuction returns a float, so we can return the value out of our function here
		//float dpiScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(X, Y));

		FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
		FVector2D size = AllottedGeometry.GetLocalSize();
		FLinearColor color = borderColor;
		color.A = borderOpacity;

		// Move the top and bottom lines inward by a little bit to help dial in the corner shape
		float adjustmentNudge = .5f; // / dpiScale;

		float thickness = borderThickness; //* dpiScale;
		// Bottom
		if (bShowBottom) UWidgetBlueprintLibrary::DrawLine(Context, FVector2D(0 - borderOffset + adjustmentNudge, size.Y + borderOffset), FVector2D(size.X + borderOffset - adjustmentNudge, size.Y + borderOffset), color, true, thickness);

		// Top
		if (bShowTop) UWidgetBlueprintLibrary::DrawLine(Context, FVector2D(0 - borderOffset + adjustmentNudge, 0 - borderOffset), FVector2D(size.X + borderOffset - adjustmentNudge, 0 - borderOffset), color, true, thickness);

		// Right
		if (bShowRight) UWidgetBlueprintLibrary::DrawLine(Context, FVector2D(size.X + borderOffset, 0 - borderOffset + (thickness / 2)), FVector2D(size.X + borderOffset, size.Y + borderOffset - (thickness / 2)), color, true, thickness);

		// Left
		if (bShowLeft) UWidgetBlueprintLibrary::DrawLine(Context, FVector2D(0 - borderOffset, 0 - borderOffset + (thickness / 2)), FVector2D(0 - borderOffset, size.Y + borderOffset - (thickness / 2)), color, true, thickness);
	}


	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}