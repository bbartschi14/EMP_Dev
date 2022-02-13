// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EMPCombatSkillsDatabase.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPCombatSkillsDatabase : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class UEMPCombatSkill>> CombatSkills;
};
