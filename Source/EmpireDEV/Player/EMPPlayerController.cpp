// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "../Game/BasicLevelGameModeEMP.h"

void AEMPPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("CancelAction", IE_Pressed, this, &AEMPPlayerController::HandleCancelActionPressed);
}

void AEMPPlayerController::HandleCancelActionPressed()
{
    ABasicLevelGameModeEMP* basicLevelMode = Cast<ABasicLevelGameModeEMP>(UGameplayStatics::GetGameMode(GetWorld()));
    basicLevelMode->HandleCancelActionPressed();
}
