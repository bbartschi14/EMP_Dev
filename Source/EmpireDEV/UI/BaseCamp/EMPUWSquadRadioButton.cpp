// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWSquadRadioButton.h"
#include "../Core/UserWidgets/Buttons/EMPUWTextButtonBase.h"

void UEMPUWSquadRadioButton::SetSquadData(UEMPSquadData* inSquad)
{
	SquadRepresented = inSquad;
	HandleSquadDataSet();
}

UEMPSquadData* UEMPUWSquadRadioButton::GetSquadData() const
{
	return SquadRepresented;
}
