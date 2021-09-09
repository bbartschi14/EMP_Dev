// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPHUD.h"
#include <GameFramework/PlayerController.h>
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "../Game/BasicLevelGameModeEMP.h"
#include "../Game/EMPBetweenGameMenuMode.h"

void AEMPHUD::ShowCombatUI()
{
	// Make widget owned by our PlayerController
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	CurrentRoot = CreateWidget<UUserWidget>(PC, CombatUIClass);

	CurrentRoot->AddToViewport();
}

void AEMPHUD::ShowBetweenGameUI()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	CurrentRoot = CreateWidget<UUserWidget>(PC, BetweenGameMenuClass);

	CurrentRoot->AddToViewport();
}


void AEMPHUD::HideUI()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<AEMPHUD>();

	if (CurrentRoot)
	{
		CurrentRoot->RemoveFromViewport();
		CurrentRoot = nullptr;
	}
}

void AEMPHUD::BeginPlay()
{
	ABasicLevelGameModeEMP* basicLevelMode = Cast<ABasicLevelGameModeEMP>(UGameplayStatics::GetGameMode(GetWorld()));
	if (basicLevelMode)
	{
		ShowCombatUI();
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("When loading main UI, game mode was not ABasicLevelGameModeEMP"));
	}

	AEMPBetweenGameMenuMode* betweenGameMode = Cast<AEMPBetweenGameMenuMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (betweenGameMode)
	{
		ShowBetweenGameUI();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("When loading main UI, game mode was not AEMPBetweenGameMenuMode"));
	}
}
