// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "EMPCombatAction.h"
#include "EMPMovementAction.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPMovementAction : public UEMPCombatAction
{
	GENERATED_BODY()
	
public:

	virtual void ExecuteAction(class ABasicLevelGameModeEMP* owningGameMode) override;

	UFUNCTION()
		void SetSquad(class AEMPSquad* squadToMove);

	UFUNCTION()
		void SetDestinationAreaCoordinate(FIntPoint areaCoordinate);

private:
	UPROPERTY(Transient)
	class AEMPSquad* SquadToMove;

	FIntPoint DestinationAreaCoordinate;
};
