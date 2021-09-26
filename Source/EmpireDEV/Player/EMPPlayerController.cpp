// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "../Game/BasicLevelGameModeEMP.h"
#include "../Game/EMPCombatMapGameMode.h"

void AEMPPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("CancelAction", IE_Pressed, this, &AEMPPlayerController::HandleCancelActionPressed);
    ClickEventKeys.Add(EKeys::RightMouseButton);
}

void AEMPPlayerController::HandleCancelActionPressed()
{
    AEMPCombatMapGameMode* combatMapMode = Cast<AEMPCombatMapGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (combatMapMode)
    {
        combatMapMode->HandleCancelActionPressed();
    }
}
