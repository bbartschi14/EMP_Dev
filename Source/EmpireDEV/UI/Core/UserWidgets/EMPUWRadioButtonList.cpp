// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWRadioButtonList.h"
#include "Buttons/EMPUWRadioButton.h"
#include "Components/PanelWidget.h"
#include "Buttons/EMPUWButtonBase.h"

void UEMPUWRadioButtonList::AddRadioButtonToList(UEMPUWRadioButton* radioButtonWidget, bool bManualAddToWidget)
{
	if (!bManualAddToWidget) RadioButtonsContainer->AddChild(radioButtonWidget);
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
			if (currentIndex == indexToSelect) SetAsSelected(radioButton);
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

void  UEMPUWRadioButtonList::RemoveRadioButton(class UEMPUWRadioButton* radioButtonToRemove)
{
	for (UWidget* child : RadioButtonsContainer->GetAllChildren())
	{
		UEMPUWRadioButton* radioButton = Cast<UEMPUWRadioButton>(child);
		if (radioButton && radioButton == radioButtonToRemove)
		{
			// Found button to remove. Check if it is currently the selected one
			if (radioButton == SelectedRadioButton) ClearSelected();

			radioButtonToRemove->RemoveFromParent();
			break;
		}
	}
}


void UEMPUWRadioButtonList::HandleRadioButtonClicked(UEMPUWRadioButton* radioButtonClicked)
{
	OnRadioButtonSelected.Broadcast(radioButtonClicked, GetIndexOfButton(radioButtonClicked));
	if (bTogglesSelf)
	{
		SetAsSelected(radioButtonClicked);
	}
}

void UEMPUWRadioButtonList::SetAsSelected(class UEMPUWRadioButton* buttonToSetSelected)
{
	if (SelectedRadioButton) SelectedRadioButton->SetToggleOnOff(false);

	buttonToSetSelected->SetToggleOnOff(true);
	SelectedRadioButton = buttonToSetSelected;
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
