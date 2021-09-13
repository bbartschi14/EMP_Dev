// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWRadioButtonList.h"
#include "Buttons/EMPUWRadioButton.h"
#include "Components/PanelWidget.h"
#include "Buttons/EMPUWButtonBase.h"

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

void UEMPUWRadioButtonList::SetSelectedIndex(int32 indexToSelect)
{
	ClearSelected();

	int32 currentIndex = 0;
	for (UWidget* child : RadioButtonsContainer->GetAllChildren())
	{
		UEMPUWRadioButton* radioButton = Cast<UEMPUWRadioButton>(child);
		if (radioButton)
		{
			if (currentIndex == indexToSelect) HandleRadioButtonClicked(radioButton);
			currentIndex++;
		}
	}
}

TArray<UEMPUWRadioButton*> UEMPUWRadioButtonList::GetRadioButtons() const
{
	TArray<UEMPUWRadioButton*> radioButtons;
	for (UWidget* child : RadioButtonsContainer->GetAllChildren())
	{
		UEMPUWRadioButton* radioButton = Cast<UEMPUWRadioButton>(child);
		if (radioButton)
		{
			radioButtons.Add(radioButton);
		}
	}
	return radioButtons;
}


UEMPUWRadioButton* UEMPUWRadioButtonList::GetSelectedRadioButton()
{
	return SelectedRadioButton;
}

void UEMPUWRadioButtonList::HandleRadioButtonClicked(UEMPUWRadioButton* radioButtonClicked)
{
	OnRadioButtonSelected.Broadcast(radioButtonClicked, GetIndexOfButton(radioButtonClicked));

	if (bTogglesSelf)
	{
		if (SelectedRadioButton) SelectedRadioButton->SetToggleOnOff(false);

		radioButtonClicked->SetToggleOnOff(true);
		SelectedRadioButton = radioButtonClicked;
	}
}

int32 UEMPUWRadioButtonList::GetIndexOfButton(UEMPUWRadioButton* buttonToGetIndex)
{
	int32 currentIndex = 0;
	for (UWidget* child : RadioButtonsContainer->GetAllChildren())
	{
		UEMPUWRadioButton* radioButton = Cast<UEMPUWRadioButton>(child);
		if (radioButton)
		{
			if (radioButton == buttonToGetIndex) return currentIndex;
			currentIndex++;
		}
	}
	return -1;
}
