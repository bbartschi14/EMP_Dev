// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWNumberPropertyDisplay.h"
#include "EMPUWText.h"
#include "Kismet/KismetTextLibrary.h"

void UEMPUWNumberPropertyDisplay::SetNumber(float inNumber)
{
	if (NumberLabel)
	{
		Number = inNumber;
		NumberLabel->SetText(UKismetTextLibrary::Conv_FloatToText(Number, ERoundingMode::HalfToEven, false, true, 1, 324, DecimalPlaces, DecimalPlaces));
	}
}

void UEMPUWNumberPropertyDisplay::SetText(FText inText)
{
	if (TextLabel)
	{
		Text = inText;
		TextLabel->SetText(Text);
	}
}

void UEMPUWNumberPropertyDisplay::SetDecimalPlaces(int32 inDecimalPlaces)
{
	DecimalPlaces = inDecimalPlaces;
}

void UEMPUWNumberPropertyDisplay::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	SetText(Text);
	SetNumber(Number);
}
