// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWRadioButton.h"
#include "Components/Button.h"

void UEMPUWRadioButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (MainButton)
	{
		// Remove first to avoid duplicate
		MainButton->OnClicked.RemoveDynamic(this, &UEMPUWRadioButton::HandleMainButtonClicked);
		MainButton->OnClicked.AddUniqueDynamic(this, &UEMPUWRadioButton::HandleMainButtonClicked);
	}
}

void UEMPUWRadioButton::HandleMainButtonClicked()
{
	OnRadioButtonClicked.Broadcast(this);
}

void UEMPUWRadioButton::SetToggleOnOff(bool isOn)
{
	bIsToggleOn = isOn;
	HandleToggleOnOff(bIsToggleOn);
}

void UEMPUWRadioButton::HandleToggleOnOff_Implementation(bool isOn)
{
	HandleToggleOnOffInternal(isOn);
}

void UEMPUWRadioButton::HandleToggleOnOffInternal(bool isOn)
{
}