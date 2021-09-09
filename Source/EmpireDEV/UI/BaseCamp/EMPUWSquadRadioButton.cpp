// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWSquadRadioButton.h"
#include "Components/TextBlock.h"

void UEMPUWSquadRadioButton::SetSquadData(UEMPSquadData* inSquad)
{
	SquadRepresented = inSquad;

	NativeText->SetText(FText::FromString(SquadRepresented->SquadName));
}

UEMPSquadData* UEMPUWSquadRadioButton::GetSquadData() const
{
	return SquadRepresented;
}
