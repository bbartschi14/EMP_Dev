// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "FEMPCombatUnitData.h"
#include "Engine/DataAsset.h"
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

UCLASS(BlueprintType)
class EMPIREDEV_API UEMPDefaultSkillsData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<EEMPCombatClass, FEMPDefaultSkills> DefaultCombatSkillsMap;
};
