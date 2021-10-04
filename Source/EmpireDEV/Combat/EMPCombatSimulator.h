// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs/FEMPCombatHitResult.h"
#include "EMPCombatSimulator.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class EMPIREDEV_API UEMPCombatSimulator : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void InitializeCombatData(class UEMPSquadData* squadOne, class UEMPSquadData* squadTwo, class AEMPCombatMapGameMode* owningCombatGameMode);

	/** Calculates attacks from both squads to eachother */
	UFUNCTION(BlueprintCallable)
		void SimulateSingleRoundOfAttacks();

	/** Returns true if any attack animation requests were broadcasted */
	UFUNCTION(BlueprintCallable)
		bool AnimateAttacks(float animationTime);

	UFUNCTION(BlueprintCallable)
		void ResolveDamage();

	UFUNCTION(BlueprintCallable)
		void UpdateCombatMovement(float animationTime);

	UFUNCTION(BlueprintCallable)
		void ResetCombatUnitsAfterCombat(float animationTime);

	UFUNCTION(BlueprintCallable)
		void RetreatUnits(class UEMPSquadData* squad, FIntPoint squadOrigin, FIntPoint attackingDirection, float animationTime);

	UFUNCTION(BlueprintCallable)
		void PerformMoraleRolls(float animationTime);

	// Returns true if any squads are animating after resolving the morale rolls
	UFUNCTION(BlueprintCallable)
		bool ResolveMoraleRolls(float animationTime);

protected:

	/** Calculates all data from attacks from one squad to another. Doesn't resolve damage yet, as all attacks technically occur simultaneously. */
	UFUNCTION()
		void SimulateSquadAttacks(class UEMPSquadData* attackingSquad, class UEMPSquadData* defendingSquad, FIntPoint attackingDirection);

	TArray<class UEMPCombatUnitData*> GetUnitsInLine(class UEMPSquadData* squad, FIntPoint origin, FIntPoint attackingDirection, int lineIndex);

	UPROPERTY(Transient, BlueprintReadOnly)
		class UEMPSquadData* CombatSquadOne;

	UPROPERTY(Transient, BlueprintReadOnly)
		class UEMPSquadData* CombatSquadTwo;

	UPROPERTY(Transient, BlueprintReadOnly)
		class AEMPCombatMapGameMode* OwningCombatGameMode;

	UPROPERTY(Transient, BlueprintReadOnly)
		FIntPoint SquadOneAttackingDirection;

	UPROPERTY(Transient, BlueprintReadOnly)
		FIntPoint SquadTwoAttackingDirection;

	UPROPERTY(BlueprintReadWrite)
		int32 CombatRoundsRemaining;

	UPROPERTY(Transient, BlueprintReadOnly)
		TArray<FEMPCombatHitResult> RoundHitResults;

	UPROPERTY(Transient)
		TArray<int32> MoraleRolls;

};
