// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EMPSquad.generated.h"

UCLASS()
class EMPIREDEV_API AEMPSquad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEMPSquad();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** */
	UFUNCTION()
		class AEMPCombatUnit* CreateCombatUnit(TSubclassOf<class AEMPCombatUnit> CombatUnitClass);

	/** */
	UFUNCTION()
		TArray<class AEMPCombatUnit*> GetCombatUnitsInSquad() const;

	/** Used in combat to advance troop by a grid space. Returns true if any unit moves */
	UFUNCTION()
		bool AdvanceCombatUnits_SingleLine(FIntPoint direction, int lineIndex, int numberOfSpacesToMove, bool bShouldNotifyWhenFinished);

	/**  */
	UFUNCTION()
		bool ResetCombatUnitsAfterCombat(FIntPoint direction, bool bShouldNotifyWhenFinished);

	/** */
	UFUNCTION()
		void SetStartingAreaCoordinate(FIntPoint areaCoordinate);

	UFUNCTION()
		void SetSquadData(class UEMPSquadData* squadData);

	UFUNCTION()
		class UEMPSquadData* GetSquadData() const;

	/** */
	UFUNCTION()
		void MoveToAreaCoordinate(FIntPoint areaCoordinate);

	/** */
	UFUNCTION()
		void HandleMovementFailed();

	/** */
	UFUNCTION(BlueprintCallable)
		FIntPoint GetCurrentAreaCoordinate() const;

	UFUNCTION()
		class AEMPCombatUnit* GetCombatUnitAtGridCoordinate(FIntPoint gridCoordinate);

	UFUNCTION()
		void SetCurrentSquadState(ESquadStateEMP newState);

	/** */
	UFUNCTION(BlueprintCallable)
		ESquadStateEMP GetCurrentSquadState() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	UFUNCTION()
		void HandleCombatUnitDeath(class AEMPCombatUnit* combatUnit);

	UPROPERTY(Transient)
		TArray<class AEMPCombatUnit*> CombatUnits;

	UPROPERTY()
		FIntPoint CurrentAreaCoordinate;

	UPROPERTY()
		ESquadStateEMP CurrentSquadState;

	UPROPERTY()
		bool bEnableDebugMode = false;

	UPROPERTY()
		class UEMPSquadData* SquadData;
};
