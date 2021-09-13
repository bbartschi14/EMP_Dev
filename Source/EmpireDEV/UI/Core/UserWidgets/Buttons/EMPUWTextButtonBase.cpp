// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWTextButtonBase.h"
#include "../EMPUWText.h"

void UEMPUWTextButtonBase::SynchronizeProperties()
{
	SetButtonText(InnerText);
	SetTextJustification(TextJustification);
	SetButtonSize(ButtonSize);

	Super::SynchronizeProperties();
}

void UEMPUWTextButtonBase::SetButtonText(FText inText)
{
	if (InnerTextLabel)
	{
		InnerText = inText;
		InnerTextLabel->SetText(InnerText);
	}
}

void UEMPUWTextButtonBase::SetTextJustification(TEnumAsByte<ETextJustify::Type> inJustification)
{
	if (InnerTextLabel)
	{
		TextJustification = inJustification;
		InnerTextLabel->SetJustification(TextJustification);
	}
}

void UEMPUWTextButtonBase::SetButtonSize(EEMPFontSize inSize)
{
	if (InnerTextLabel)
	{
		ButtonSize = inSize;
		InnerTextLabel->SetFontSize(ButtonSize);
	}
}
