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
		void SpawnCombatSquad(class UEMPSquadData* OwningSquad);

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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, CallInEditor)
		void LoadMapFromGameInstance();

	UFUNCTION()
		TMap<FIntPoint, bool> GetSpawnPoints() const { return SpawnPoints; }

	UFUNCTION()
		TArray<class UCombatObjective*> GetCombatObjectives() const { return CombatObjectives; }

	UFUNCTION(BlueprintCallable)
		void LoadFromDataAsset(class UCombatMapDataAsset* InData);

	void MapInitialize() override;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void SpawnCombatUnit(class UEMPCombatUnitData* combatUnitData, FEMPCombatUnitMeshData meshData);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TMap<FIntPoint, bool> SpawnPoints; // Maps location to friendly (true), enemy (false)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
		TArray<class UCombatObjective*> CombatObjectives;

	UPROPERTY(Transient, BlueprintReadOnly)
		TArray<class AEMPCombatSquad*> CombatSquads;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class AEMPCombatSquad> CombatSquadClass;


};
