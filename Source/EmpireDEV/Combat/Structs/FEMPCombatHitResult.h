// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "FEMPCombatUnitData.h"
#include "FEMPCombatHitResult.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEMPCombatHitResult
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(Transient, BlueprintReadOnly)
		UEMPCombatUnitData* AttackingUnit;

	UPROPERTY(Transient, BlueprintReadOnly)
		UEMPCombatUnitData* DefendingUnit;

	UPROPERTY(Transient, BlueprintReadOnly)
		bool bHitSuccessful;

	UPROPERTY(Transient, BlueprintReadOnly)
		int32 DamageDealt;
};
