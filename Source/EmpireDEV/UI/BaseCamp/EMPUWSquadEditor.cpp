// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWSquadEditor.h"
#include "../../Subsystems/EMPSquadManager.h"
#include "EMPUWSquadRadioButton.h"
#include "../Core/UserWidgets/EMPUWRadioButtonList.h"
#include <Kismet/GameplayStatics.h>
#include "EMPUWCombatUnitInspector.h"

void UEMPUWSquadEditor::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	SynchSquadData();

	if (SquadRadioButtonList)
	{
		// Remove first to avoid duplicate
		SquadRadioButtonList->OnRadioButtonSelected.RemoveDynamic(this, &UEMPUWSquadEditor::HandleRadioButtonSelected);
		SquadRadioButtonList->OnRadioButtonSelected.AddUniqueDynamic(this, &UEMPUWSquadEditor::HandleRadioButtonSelected);
	}

	AEMPBetweenGameMenuMode* betweenGameMode = Cast<AEMPBetweenGameMenuMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (betweenGameMode)
	{
		betweenGameMode->OnCombatUnitSelected.RemoveDynamic(this, &UEMPUWSquadEditor::HandleCombatUnitSelected);
		betweenGameMode->OnCombatUnitSelected.AddUniqueDynamic(this, &UEMPUWSquadEditor::HandleCombatUnitSelected);

		betweenGameMode->OnGameStateChanged.RemoveDynamic(this, &UEMPUWSquadEditor::HandleGameStateChanged);
		betweenGameMode->OnGameStateChanged.AddUniqueDynamic(this, &UEMPUWSquadEditor::HandleGameStateChanged);

		betweenGameMode->OnSquadDeselected.RemoveDynamic(this, &UEMPUWSquadEditor::HandleSquadDeselected);
		betweenGameMode->OnSquadDeselected.AddUniqueDynamic(this, &UEMPUWSquadEditor::HandleSquadDeselected);
	}
}

void UEMPUWSquadEditor::NativeConstruct()
{
	Super::NativeConstruct();
}


void UEMPUWSquadEditor::SynchSquadData()
{
	UEMPSquadManager* squadManager = GetWorld()->GetGameInstance()->GetSubsystem<UEMPSquadManager>();
	if (!squadManager) return;

	TArray<UEMPSquadData*> squadDataArray = squadManager->GetSquadData();
	SquadRadioButtonList->ClearList();

	for (UEMPSquadData* singleSquadData : squadDataArray)
	{
		UEMPUWSquadRadioButton* squadRadioButton = CreateWidget<UEMPUWSquadRadioButton>(GetOwningPlayer(), SquadRadioButtonClass);
		SquadRadioButtonList->AddRadioButtonToList(squadRadioButton);
		squadRadioButton->SetSquadData(singleSquadData);
	}
}

void UEMPUWSquadEditor::HandleRadioButtonSelected(UEMPUWRadioButton* radioButton)
{
	AEMPBetweenGameMenuMode* betweenGameMode = Cast<AEMPBetweenGameMenuMode>(UGameplayStatics::GetGameMode(GetWorld()));
	UEMPUWSquadRadioButton* squadRadioButton = Cast<UEMPUWSquadRadioButton>(radioButton);
	if (betweenGameMode && squadRadioButton)
	{
		betweenGameMode->SelectSquad(squadRadioButton->GetSquadData());
	}
}

void UEMPUWSquadEditor::HandleCombatUnitSelected(class UEMPCombatUnitData* combatUnitData)
{
	CombatUnitInspector->SetCombatUnitData(combatUnitData);
}

void UEMPUWSquadEditor::HandleGameStateChanged(EBaseCampGameStateEMP newState)
{
	if (newState == EBaseCampGameStateEMP::BC_EDITING_UNIT || newState == EBaseCampGameStateEMP::BC_MOVING_UNIT)
	{
		CombatUnitInspector->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		CombatUnitInspector->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void UEMPUWSquadEditor::HandleSquadDeselected()
{
	SquadRadioButtonList->ClearSelected();
}
