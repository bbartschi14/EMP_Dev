// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "FEMPCombatHitResult.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEMPCombatHitResult
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(Transient, BlueprintReadWrite)
		class UEMPCombatUnitData* AttackingUnit;

	UPROPERTY(Transient, BlueprintReadWrite)
		class UEMPCombatUnitData* DefendingUnit;

	UPROPERTY(Transient, BlueprintReadWrite)
		bool bHitSuccessful;

	UPROPERTY(Transient, BlueprintReadWrite)
		int32 DamageDealt;
};
