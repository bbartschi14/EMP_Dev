// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWSquadRadioButton.h"
#include "../Core/UserWidgets/EMPUWTextButtonTemplate.h"

void UEMPUWSquadRadioButton::SetSquadData(UEMPSquadData* inSquad)
{
	SquadRepresented = inSquad;

	UEMPUWTextButtonTemplate* textButton = Cast<UEMPUWTextButtonTemplate>(MainButton);
	if (textButton)
	{
		textButton->SetButtonText(FText::FromString(SquadRepresented->SquadName));
	}
}

UEMPSquadData* UEMPUWSquadRadioButton::GetSquadData() const
{
	return SquadRepresented;
}
