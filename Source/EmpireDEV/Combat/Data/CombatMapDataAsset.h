// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CombatMapDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class EMPIREDEV_API UCombatMapDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int GridSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FIntPoint, bool> SpawnPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	TArray<class UCombatObjective*> CombatObjectives;

	UFUNCTION(BlueprintCallable)
	void CopyFromGrid(class AEMPCombatMapGrid* InGrid);
};
