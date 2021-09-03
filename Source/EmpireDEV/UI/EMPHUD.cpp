// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPHUD.h"
#include <GameFramework/PlayerController.h>
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "../Game/BasicLevelGameModeEMP.h"

void AEMPHUD::ShowMainUI()
{
	// Make widget owned by our PlayerController
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	MainUI = CreateWidget<UUserWidget>(PC, MainUIClass);

	MainUI->AddToViewport();
}

void AEMPHUD::HideMainUI()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<AEMPHUD>();

	if (MainUI)
	{
		MainUI->RemoveFromViewport();
		MainUI = nullptr;
	}
}

void AEMPHUD::BeginPlay()
{
	ABasicLevelGameModeEMP* basicLevelMode = Cast<ABasicLevelGameModeEMP>(UGameplayStatics::GetGameMode(GetWorld()));
	if (basicLevelMode)
	{
		ShowMainUI();
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("When loading main UI, game mode was not ABasicLevelGameModeEMP"));
	}
}
