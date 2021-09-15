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
	ClearSelectedCombatUnit();

	if (SelectedSquad)
	{
		SelectedSquad = nullptr;
		OnSquadDeselected.Broadcast();
	}
}

UEMPSquadData* AEMPBaseCampGameMode::GetSelectedSquad() const
{
	return SelectedSquad;
}

void AEMPBaseCampGameMode::SelectCombatUnit(UEMPCombatUnitData* combatUnitToLoad)
{
	ClearSelectedCombatUnit();
	SelectedCombatUnit = combatUnitToLoad;
	OnCombatUnitSelected.Broadcast(SelectedCombatUnit);
}

void AEMPBaseCampGameMode::ClearSelectedCombatUnit()
{
	if (SelectedCombatUnit)
	{
		SelectedCombatUnit = nullptr;
		OnCombatUnitDeselected.Broadcast();
	}
}

void AEMPBaseCampGameMode::HandleSquadDissolved(class UEMPSquadData* squadDissolved)
{
	if (squadDissolved == SelectedSquad)
	{
		ClearSelectedSquad();
	}
}

void AEMPBaseCampGameMode::HandleCombatUnitRemovedFromSquad(UEMPCombatUnitData* combatUnitRemoved, UEMPSquadData* squad)
{
	if (SelectedCombatUnit && combatUnitRemoved == SelectedCombatUnit)
	{
		ClearSelectedCombatUnit();
	}
}

void AEMPBaseCampGameMode::BeginPlay()
{
	Super::BeginPlay();

	UEMPSquadManager* squadManager = GetWorld()->GetGameInstance()->GetSubsystem<UEMPSquadManager>();
	if (squadManager)
	{
		squadManager->OnSquadDissolved.AddUniqueDynamic(this, &AEMPBaseCampGameMode::HandleSquadDissolved);
		squadManager->OnCombatUnitRemovedFromSquad.AddUniqueDynamic(this, &AEMPBaseCampGameMode::HandleCombatUnitRemovedFromSquad);
	}

	SpawnGrid();
}