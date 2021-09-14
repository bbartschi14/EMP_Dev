// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPBaseCampGameMode.h"
#include "../Subsystems/EMPSquadManager.h"
void AEMPBaseCampGameMode::SelectSquad(UEMPSquadData* squadToLoad)
{
	// Start by clearing previous selection
	ClearSelectedSquad();

	// Assign reference and broadcast
	SelectedSquad = squadToLoad;
	OnSquadSelected.Broadcast(SelectedSquad);
}

void AEMPBaseCampGameMode::ClearSelectedSquad()
{
	if (SelectedSquad)
	{
		SelectedSquad = nullptr;
		OnSquadDeselected.Broadcast();
	}
}

void AEMPBaseCampGameMode::HandleSquadDissolved(class UEMPSquadData* squadDissolved)
{
	if (squadDissolved == SelectedSquad)
	{
		ClearSelectedSquad();
	}
}



void AEMPBaseCampGameMode::BeginPlay()
{
	Super::BeginPlay();

	UEMPSquadManager* squadManager = GetWorld()->GetGameInstance()->GetSubsystem<UEMPSquadManager>();
	if (squadManager)
	{
		squadManager->OnSquadDissolved.AddUniqueDynamic(this, &AEMPBaseCampGameMode::HandleSquadDissolved);
	}

	SpawnGrid();
}