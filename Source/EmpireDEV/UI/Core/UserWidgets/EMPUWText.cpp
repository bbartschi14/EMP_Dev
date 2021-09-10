// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWText.h"
#include "Components/TextBlock.h"

void UEMPUWText::SynchronizeProperties()
{
	if (NativeText)
	{
		SetText(Text);
		SetJustification(Justification);
		SetFontSize(FontSize);
	}
	Super::SynchronizeProperties();
}

void UEMPUWText::SetText(const FText& InText)
{
	Text = InText;
	if (NativeText)
	{
		NativeText->SetText(Text);
	}
}

const FText& UEMPUWText::GetText() const
{
	return Text;
}

void UEMPUWText::SetFontSize(int32 InFontSize)
{
	FontSize = InFontSize;
	if (NativeText) {
		FSlateFontInfo newFont = NativeText->Font;
		newFont.Size = FontSize;
		NativeText->SetFont(newFont);
	}
}

void UEMPUWText::SetJustification(TEnumAsByte<ETextJustify::Type> InJustification)
{
	Justification = InJustification;
	if (NativeText) {
		NativeText->SetJustification(Justification);
	}
}
