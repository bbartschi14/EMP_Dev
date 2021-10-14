// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWRadioButton.h"
#include "EMPUWButtonBase.h"

void UEMPUWRadioButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (Button)
	{
		Button->OnClick.AddUniqueDynamic(this, &UEMPUWRadioButton::HandleButtonClicked);
		Button->OnHover.AddUniqueDynamic(this, &UEMPUWRadioButton::HandleButtonHovered);
		Button->OnUnhover.AddUniqueDynamic(this, &UEMPUWRadioButton::HandleButtonUnhovered);
	}
}

void UEMPUWRadioButton::SetToggleOnOff(bool isOn)
{
	Button->SetToggleOnOff(isOn);
	HandleToggleOnOff(isOn);
}

void UEMPUWRadioButton::HandleButtonClicked()
{
	OnRadioButtonClicked.Broadcast(this);
}

void UEMPUWRadioButton::HandleButtonHovered()
{
	OnRadioButtonHovered.Broadcast(this);
}

void UEMPUWRadioButton::HandleButtonUnhovered()
{
	OnRadioButtonUnhovered.Broadcast(this);
}