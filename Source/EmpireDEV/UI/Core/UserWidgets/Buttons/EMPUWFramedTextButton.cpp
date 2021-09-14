// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWFramedTextButton.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "../EMPUWAnimatedColoredFrame.h"

void UEMPUWFramedTextButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	SetButtonColor(ButtonColor);
	SetUseOptionalImage(bUseOptionalIcon);
	SetOptionalImageResource(OptionalIconImage);
	SetFadeFrame(bHideFrame);
}

void UEMPUWFramedTextButton::SetButtonColor(FLinearColor inColor)
{
	if (NativeButton)
	{
		ButtonColor = inColor;
		NativeButton->SetBackgroundColor(inColor);
	}
}

void UEMPUWFramedTextButton::SetUseOptionalImage(bool inValue)
{
	if (IconImage)
	{
		bUseOptionalIcon = inValue;
		IconImage->SetVisibility(bUseOptionalIcon ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}

}

void UEMPUWFramedTextButton::SetOptionalImageResource(UObject* inObject)
{
	if (IconImage)
	{
		OptionalIconImage = inObject;
		IconImage->SetBrushResourceObject(OptionalIconImage);
	}
}

void UEMPUWFramedTextButton::SetFadeFrame(bool inHideFrame)
{
	if (ColoredFrame)
	{
		bHideFrame = inHideFrame;
		if (bHideFrame)
		{
			ColoredFrame->SetStaticBorderOpacity(0.35f);
		}
		else
		{
			ColoredFrame->SetStaticBorderOpacity(1.0f);
		}
	}
}
