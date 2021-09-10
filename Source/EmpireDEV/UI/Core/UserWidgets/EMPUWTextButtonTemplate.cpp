// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWTextButtonTemplate.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ButtonSlot.h"
#include "EMPUWText.h"
#include "EMPUWAnimatedColoredFrame.h"

void UEMPUWTextButtonTemplate::SynchronizeProperties()
{
	if (InnerTextLabel)
	{
		SetButtonSize(ButtonSize);
		SetButtonText(InnerText);
		SetTextJustification(TextJustification);
		SetButtonColor(ButtonColor);
		SetUseOptionalImage(bUseOptionalIcon);
		SetOptionalImageResource(OptionalIconImage);
		SetStayActive(bIsStayActive);
	}

	Super::SynchronizeProperties();
}

void UEMPUWTextButtonTemplate::SetButtonText(FText inText)
{
	InnerText = inText;
	if (InnerTextLabel)
	{
		InnerTextLabel->SetText(InnerText);
	}
}

void UEMPUWTextButtonTemplate::SetTextJustification(TEnumAsByte<ETextJustify::Type> inJustification)
{
	TextJustification = inJustification;
	if (InnerTextLabel)
	{
		InnerTextLabel->SetJustification(TextJustification);
	}
}

void UEMPUWTextButtonTemplate::SetButtonSize(int32 inSize)
{
	ButtonSize = inSize;
	if (InnerTextLabel)
	{
		InnerTextLabel->SetFontSize(ButtonSize);
	}

	if (PaddedOverlay)
	{
		UButtonSlot* OverlayAsSlot = Cast<UButtonSlot>(PaddedOverlay->Slot);
		if (OverlayAsSlot)
		{
			float marginSize = 6.f;
			OverlayAsSlot->SetPadding(FMargin(marginSize, marginSize, marginSize, marginSize));
		}
	}
}

void UEMPUWTextButtonTemplate::SetButtonColor(FLinearColor inColor)
{
	ButtonColor = inColor;
	if (NativeButton)
	{
		NativeButton->SetBackgroundColor(inColor);
	}
}

void UEMPUWTextButtonTemplate::SetUseOptionalImage(bool inValue)
{
	bUseOptionalIcon = inValue;
	if (IconImage)
	{
		IconImage->SetVisibility(bUseOptionalIcon ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}

}

void UEMPUWTextButtonTemplate::SetOptionalImageResource(UObject* inObject)
{
	OptionalIconImage = inObject;
	if (IconImage)
	{
		IconImage->SetBrushResourceObject(OptionalIconImage);
	}
}

void UEMPUWTextButtonTemplate::SetStayActive(bool inValue)
{
	bIsStayActive = inValue;

	if (ColoredFrame)
	{
		if (bIsStayActive)
		{
			ColoredFrame->NativeActivateBorder();
		}
		else
		{
			ColoredFrame->NativeDeactivateBorder();
		}
	}

	if (NativeButton)
	{
		if (bIsStayActive)
		{
			// Darken the background color on stay active
			NativeButton->SetBackgroundColor(ButtonColor * .8f);
		}
		else
		{
			NativeButton->SetBackgroundColor(ButtonColor);
		}
	}
}

