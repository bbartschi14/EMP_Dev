// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EMPCombatAction.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPCombatAction : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
		virtual void ExecuteAction(class ABasicLevelGameModeEMP* owningGameMode);
};
