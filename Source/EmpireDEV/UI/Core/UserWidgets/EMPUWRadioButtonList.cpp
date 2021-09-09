// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWRadioButtonList.h"
#include "EMPUWRadioButton.h"
#include "Components/PanelWidget.h"

void UEMPUWRadioButtonList::AddRadioButtonToList(UEMPUWRadioButton* radioButtonWidget)
{
	RadioButtonsContainer->AddChild(radioButtonWidget);
	radioButtonWidget->OnRadioButtonClicked.AddUniqueDynamic(this, &UEMPUWRadioButtonList::HandleRadioButtonClicked);
}

void UEMPUWRadioButtonList::ClearList()
{
	SelectedRadioButton = nullptr;
	RadioButtonsContainer->ClearChildren();
}

void UEMPUWRadioButtonList::ClearSelected()
{
	if (SelectedRadioButton)
	{
		SelectedRadioButton->SetToggleOnOff(false);
		SelectedRadioButton = nullptr;
	}
}

void UEMPUWRadioButtonList::HandleRadioButtonClicked(UEMPUWRadioButton* radioButtonClicked)
{
	if (SelectedRadioButton) SelectedRadioButton->SetToggleOnOff(false);

	radioButtonClicked->SetToggleOnOff(true);
	SelectedRadioButton = radioButtonClicked;
	OnRadioButtonSelected.Broadcast(SelectedRadioButton);
}

