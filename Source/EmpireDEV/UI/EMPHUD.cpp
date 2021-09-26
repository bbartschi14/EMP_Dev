// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPHUD.h"
#include <GameFramework/PlayerController.h>
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "../Game/BasicLevelGameModeEMP.h"
#include "../Game/EMPBaseCampGameMode.h"
#include "../Game/EMPCombatMapGameMode.h"

void AEMPHUD::ShowCombatUI()
{
	// Make widget owned by our PlayerController
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	CurrentRoot = CreateWidget<UUserWidget>(PC, CombatUIClass);

	CurrentRoot->AddToViewport();
}

void AEMPHUD::ShowBaseCampUI()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	CurrentRoot = CreateWidget<UUserWidget>(PC, BaseCampMenuClass);

	CurrentRoot->AddToViewport();
}

void AEMPHUD::ShowCombatMapUI()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	CurrentRoot = CreateWidget<UUserWidget>(PC, CombatMapMenuClass);

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
	Super::BeginPlay();

	ABasicLevelGameModeEMP* basicLevelMode = Cast<ABasicLevelGameModeEMP>(UGameplayStatics::GetGameMode(GetWorld()));
	if (basicLevelMode)
	{
		ShowCombatUI();
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("When loading main UI, game mode was not ABasicLevelGameModeEMP"));
	}

	AEMPBaseCampGameMode* baseCampGameMode = Cast<AEMPBaseCampGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (baseCampGameMode)
	{
		ShowBaseCampUI();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("When loading main UI, game mode was not AEMPBaseCampGameMode"));
	}

	AEMPCombatMapGameMode* combatMapGameMode = Cast<AEMPCombatMapGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (combatMapGameMode)
	{
		ShowCombatMapUI();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("When loading main UI, game mode was not AEMPCombatMapGameMode"));
	}
	
}
