// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameModeEMP.generated.h"

/**
* The base game mode to use for the game. 
* Inherited by:
*	- AMenuGameModeEMP (for menu navigation)
*	- AEMPBetweenGameMenuMode (for squad management, level selection)
*	- ABasicLevelGameModeEMP (for in game combat mode)
* 
***********************************************************************************/

UCLASS(Abstract, NotBlueprintable)
class EMPIREDEV_API ABaseGameModeEMP : public AGameModeBase
{
	GENERATED_BODY()
	
};
