// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CombatObjective.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, hideCategories="CombatObjective")
class EMPIREDEV_API UCombatObjective : public UObject
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveNotify, UCombatObjective*, Objective);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnObjectiveNotify OnObjectiveComplete;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	FName GetObjectiveName() const;

	/** Return description of objective */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	FText GetObjectiveText() const;

	/** Setup objective data, bind to events */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitializeObjective(class AEMPCombatMapGameMode* CombatGameMode);
};
