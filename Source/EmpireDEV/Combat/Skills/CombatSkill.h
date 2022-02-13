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
	void SetOwningSquad(class UEMPSquadData* InOwningSquad);

	UFUNCTION(BlueprintImplementableEvent)
	bool AreSkillRequirementsMet(class UEMPSquadData* Squad, class UEMPCombatUnitData* CombatUnit);

	UFUNCTION()
	virtual FName GetSkillType() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName SkillName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText SkillDescription;

protected:
	UPROPERTY(Transient, BlueprintReadOnly, VisibleAnywhere)
	class UEMPSquadData* OwningSquad;
};
