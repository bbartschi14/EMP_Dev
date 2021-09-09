// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FEMPSquadData.h"
#include "EMPSquadsDatabase.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPSquadsDatabase : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		TArray<FEMPSquadDataStruct> Squads;
};
