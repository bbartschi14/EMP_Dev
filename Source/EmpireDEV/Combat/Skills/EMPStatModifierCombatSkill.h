// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "CombatSkill.h"
#include "EMPStatModifierCombatSkill.generated.h"

USTRUCT(BlueprintType)
struct FEMPStatModifierCombatSkillData
{
	GENERATED_BODY()

public:
	/** Unit that owns this skill */
	UPROPERTY(Transient, BlueprintReadOnly)
	class UEMPCombatUnitData* OwningUnit;

	UPROPERTY(Transient, BlueprintReadOnly)
	class UEMPCombatUnitData* OtherUnit;

	/** True if owning is attacking other */
	UPROPERTY(Transient, BlueprintReadOnly)
	bool bIsAttack;
};

/**
 * 
 */
UCLASS(Blueprintable)
class EMPIREDEV_API UEMPStatModifierCombatSkill : public UEMPCombatSkill
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetData(FEMPStatModifierCombatSkillData InData);

	/** Returns new hit chance */
	UFUNCTION(BlueprintNativeEvent)
	float ModifyHitChance(float OriginalHitChance);
	virtual float ModifyHitChance_Implementation(float OriginalHitChance);

	/** Returns new damage */
	UFUNCTION(BlueprintNativeEvent)
	int ModifyDamage(int OriginalDamage);
	virtual int ModifyDamage_Implementation(int OriginalDamage);

	/** Returns new armor */
	UFUNCTION(BlueprintNativeEvent)
	int ModifyArmor(int OriginalArmor);
	int ModifyArmor_Implementation(int OriginalArmor);

	FName GetSkillType() const override;
protected:
	UPROPERTY(BlueprintReadOnly)
	FEMPStatModifierCombatSkillData Data;

};
