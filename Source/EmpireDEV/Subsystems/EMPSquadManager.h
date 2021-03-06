// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Combat/Structs/FEMPCombatUnitData.h"
#include "EMPSquadManager.generated.h"

/**
 * Used to manage the player's squad data over the entire lifetime of the game.
 * When a level is started, the game mode will pull in squad data from here.
 */
UCLASS()
class EMPIREDEV_API UEMPSquadManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

#pragma region Delegates
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSquadChanged, class UEMPSquadData*, squadCreated);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatUnitNotify, class UEMPCombatUnitData*, CombatUnit);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCombatUnitSquadInteraction, class UEMPCombatUnitData*, unit, class UEMPSquadData*, squad);

	/** Called when a new squad is created */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnSquadChanged OnSquadCreated;

	/** Called when a squad is dissolved */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnSquadChanged OnSquadDissolved;

	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnCombatUnitNotify OnUnassignedUnitCreated;

	/** Called when a combat unit is added to a squad */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnCombatUnitSquadInteraction OnCombatUnitAssignedToSquad;

	/** Called when a combat unit is removed from a squad */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnCombatUnitSquadInteraction OnCombatUnitRemovedFromSquad;

	/** Called when a combat unit is moved within a squad */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnCombatUnitSquadInteraction OnCombatUnitMovedToNewDesiredLocation;

#pragma endregion Delegates

	UFUNCTION(BlueprintCallable)
		TArray<class UEMPSquadData*> GetSquadData() const;

	UFUNCTION(BlueprintCallable)
		TArray<class UEMPCombatUnitData*> GetUnassignedCombatUnitData() const;

	/** Return true if successful */
	UFUNCTION(BlueprintCallable)
		bool RemoveCombatUnitFromSquad(class UEMPCombatUnitData* combatUnit, class UEMPSquadData* squad);

	/** Return true if successful. Combat Unit should not already be in a squad */
	UFUNCTION(BlueprintCallable)
		bool AssignCombatUnitToSquad(class UEMPCombatUnitData* combatUnit, class UEMPSquadData* squad);

	UFUNCTION(BlueprintCallable)
		void MoveCombatUnitToNewDesiredLocation(class UEMPCombatUnitData* combatUnit, FIntPoint newDesiredLocation);

	/** Create a new squad, given a random-esque name */
	UFUNCTION(BlueprintCallable)
		UEMPSquadData* CreateNewSquad();

	/** Dissolve a squad. Returns true if successful. Can fail if the squad requested doesn't match any squads in this manager */
	UFUNCTION(BlueprintCallable)
		bool DissolveSquad(class UEMPSquadData* squadToDissolve);

	UFUNCTION(BlueprintCallable)
		void CreateUnassignedUnit(FEMPCombatUnitDataStruct Unit);
private:
	UPROPERTY(Transient)
		TArray<class UEMPSquadData*> Squads;

	UPROPERTY(Transient)
		TArray<class UEMPCombatUnitData*> UnassignedCombatUnits;

	/** When loading a squad, the squad manager uses this to check that the squad data is valid.
	  * If possible, it fixes invalid data and proceeds. For example, if two units in a squad have
	  * the same desired location, it will change one of them to an available space and continue as normal.
	  */
	UFUNCTION()
		bool ValidateSquad(class UEMPSquadData* squadToValidate);

	UFUNCTION()
		bool DoesCombatUnitHaveUniqueLocationInSquad(class UEMPSquadData* squad, class UEMPCombatUnitData* originalUnit);

	UFUNCTION()
		FIntPoint FindUniqueLocationInSquad(class UEMPSquadData* squad);

	UPROPERTY()
		int32 MaxSquadSize = 15;
};
