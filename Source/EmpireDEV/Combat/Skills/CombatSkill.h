// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CombatSkill.generated.h"

/**
 * Abstract base class for combat skills
 */
UCLASS(Abstract, BlueprintType)
class EMPIREDEV_API UEMPCombatSkill : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual FName GetSkillType() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName SkillName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText SkillDescription;
};
