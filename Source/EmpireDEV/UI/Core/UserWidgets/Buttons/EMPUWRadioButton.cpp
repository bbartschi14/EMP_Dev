// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWRadioButton.h"
#include "EMPUWTextButtonBase.h"

void UEMPUWRadioButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (TextButton)
	{
		TextButton->OnClick.AddUniqueDynamic(this, &UEMPUWRadioButton::HandleTextButtonClicked);
		TextButton->OnHover.AddUniqueDynamic(this, &UEMPUWRadioButton::HandleTextButtonHovered);
		TextButton->OnUnhover.AddUniqueDynamic(this, &UEMPUWRadioButton::HandleTextButtonUnhovered);
	}
}

void UEMPUWRadioButton::SetText(FText inText)
{
	if (TextButton)
	{
		TextButton->SetButtonText(inText);
	}
}

void UEMPUWRadioButton::SetToggleOnOff(bool isOn)
{
	TextButton->SetToggleOnOff(isOn);
}

void UEMPUWRadioButton::HandleTextButtonClicked()
{
	OnRadioButtonClicked.Broadcast(this);
}

void UEMPUWRadioButton::HandleTextButtonHovered()
{
	OnRadioButtonHovered.Broadcast(this);
}

void UEMPUWRadioButton::HandleTextButtonUnhovered()
{
	OnRadioButtonUnhovered.Broadcast(this);
}