// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EMPArmyResources.h"
#include "EMPArmyDatabase.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPArmyDatabase : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FEMPArmyResourcesStruct ArmyResources;

};
