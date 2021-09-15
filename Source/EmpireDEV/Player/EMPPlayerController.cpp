// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "../Game/BasicLevelGameModeEMP.h"
#include "../Game/EMPBetweenGameMenuMode.h"

void AEMPPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("CancelAction", IE_Pressed, this, &AEMPPlayerController::HandleCancelActionPressed);
    ClickEventKeys.Add(EKeys::RightMouseButton);
}

void AEMPPlayerController::HandleCancelActionPressed()
{
    ABasicLevelGameModeEMP* basicLevelMode = Cast<ABasicLevelGameModeEMP>(UGameplayStatics::GetGameMode(GetWorld()));
    if (basicLevelMode)
    {
        basicLevelMode->HandleCancelActionPressed();
    }

    AEMPBetweenGameMenuMode* betweenGameMode = Cast<AEMPBetweenGameMenuMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (betweenGameMode)
    {
        betweenGameMode->HandleCancelActionPressed();
    }
}
