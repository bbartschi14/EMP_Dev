// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../Combat/EMPCombatUnit.h"
#include "EMPGrid.h"
#include "../Combat/Enums/EEMPCombatDirection.h"
#include "EMPCombatMapGrid.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API AEMPCombatMapGrid : public AEMPGrid
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
		void SpawnFriendlyCombatUnit(class UEMPCombatUnitData* friendlyCombatUnit);

	UFUNCTION()
		void SpawnEnemyCombatUnit(class UEMPCombatUnitData* enemyCombatUnit);

	UFUNCTION()
		TArray<FIntPoint> GetFriendlySquadSpawnPoints() const;

	UFUNCTION()
		TArray<FIntPoint> GetEnemySquadSpawnPoints() const;

	UFUNCTION(BlueprintCallable)
		void TurnCombatUnitsInSquadToDirection(class UEMPSquadData* squadData, EEMPCombatDirection direction);

	UFUNCTION(BlueprintCallable)
		void TurnCombatUnitsInSquadToDefaultDirection(class UEMPSquadData* squadData);

	UFUNCTION(BlueprintCallable)
		void SetCombatUnitsInSquadReady(class UEMPSquadData* squadData, bool isReady);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SpawnArrowIndicator(class UEMPCombatActionSkill* OwningAction, FIntPoint OriginCoordinate, FIntPoint DestinationCoordinate);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void SpawnCombatUnit(class UEMPCombatUnitData* combatUnitData, FEMPCombatUnitMeshData meshData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FIntPoint> FriendlySquadSpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FIntPoint> EnemySquadSpawnPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class AEMPCombatUnit> EnemyCombatUnitClass;
};
