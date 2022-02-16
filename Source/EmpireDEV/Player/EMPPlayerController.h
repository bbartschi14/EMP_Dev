// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EMPPlayerController.generated.h"

/**
* The player controller handles input and camera related functions.
* The controller is attached to the game mode pawn, so movement is sent through
* the controlled pawn.
***********************************************************************************/

UCLASS(Abstract, Blueprintable)
class EMPIREDEV_API AEMPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// ~ Begin Overrides: APlayerController
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
		void HandleCancelActionPressed();
};
