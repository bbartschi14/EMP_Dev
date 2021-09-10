// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWSquadInspector.h"
#include "../../Combat/Structs/FEMPSquadData.h"
#include "../Core/UserWidgets/EMPUWText.h"

void UEMPUWSquadInspector::SetSquadData(class UEMPSquadData* squad)
{
	SquadRepresented = squad;
	SquadNameText->SetText(FText::FromString(squad->SquadName));

}
