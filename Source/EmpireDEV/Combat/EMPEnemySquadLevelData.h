// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/FEMPSquadData.h"
#include "EMPEnemySquadLevelData.generated.h"

/**
 * Holds the data for the enemy squads on a level, i.e. their starting configurations and stats
 */
UCLASS(Blueprintable)
class EMPIREDEV_API AEMPEnemySquadLevelData : public AActor
{
	GENERATED_BODY()
	
public:	
	AEMPEnemySquadLevelData();

	UFUNCTION(BlueprintCallable)
		TArray<class UEMPSquadData*> GetSquadData() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FEMPSquadDataStruct> EnemySquads;

};
