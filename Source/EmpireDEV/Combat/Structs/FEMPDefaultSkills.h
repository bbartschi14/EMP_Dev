// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FEMPCombatUnitData.h"
#include "FEMPDefaultSkills.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FEMPDefaultSkills
{
	GENERATED_BODY()
public:
	FEMPDefaultSkills() {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UEMPCombatSkill>> CombatSkills;
};

USTRUCT(BlueprintType)
struct FEMPDefaultSkillsRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	FEMPDefaultSkillsRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEMPCombatClass CombatClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEMPDefaultSkills CombatSkills;
};
