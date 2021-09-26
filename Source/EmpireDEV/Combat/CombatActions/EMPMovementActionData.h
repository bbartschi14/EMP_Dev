// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EMPCombatActionData.h"
#include "EMPMovementActionData.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPMovementActionData : public UEMPCombatActionData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Transient, BlueprintReadOnly)
		class UEMPSquadData* SquadToMove;

	UPROPERTY(Transient, BlueprintReadOnly)
		FIntPoint Destination;
};
