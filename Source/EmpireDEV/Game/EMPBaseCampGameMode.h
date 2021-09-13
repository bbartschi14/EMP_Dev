// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "EMPGridGameMode.h"
#include "EMPBaseCampGameMode.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class EMPIREDEV_API AEMPBaseCampGameMode : public AEMPGridGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
