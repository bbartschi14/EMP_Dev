// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "CombatSkill.h"
#include "EMPCombatActionSkill.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class EMPIREDEV_API UEMPCombatActionSkill : public UEMPCombatSkill
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatActionFinishedExecuting, UEMPCombatActionSkill*, Action);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatActionStartedExecuting, UEMPCombatActionSkill*, Action);

	UPROPERTY(BlueprintAssignable)
	FOnCombatActionFinishedExecuting OnCombatActionFinishedExecuting;

	UPROPERTY(BlueprintAssignable)
	FOnCombatActionStartedExecuting OnCombatActionStartedExecuting;

	UFUNCTION(BlueprintCallable)
	void RequestActionQueue(class AEMPCombatMapGameMode* InOwningGameMode);

	UFUNCTION(BlueprintCallable)
	void ExecuteAction();

	UFUNCTION(BlueprintCallable)
	void CancelQueue();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HandleGridSelected(FIntPoint InGridCoordinate, FIntPoint InGridArea);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool IsGridAreaSelectable(FIntPoint InGridArea);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool IsGridCoordinateSelectable(FIntPoint InGridCoordinate);

	/** Construct a tooltip like snippet describing the queued action, e.g. "Squad One move to location 3,4" */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	FText GetQueuedActionText() const;

	// SKILL OVERRIDES
	FName GetSkillType() const override;

protected:
	/** Hook for events when the queue is requested */
	UFUNCTION(BlueprintImplementableEvent)
	void OnActionRequestQueue();

	/** Called for performing the contents of the action. Must call OnExecutionFinished */
	UFUNCTION(BlueprintImplementableEvent)
	void OnExecuteAction();

	/** */
	UFUNCTION(BlueprintCallable)
	void ConfirmQueue();

	UFUNCTION(BlueprintCallable)
	void OnExecutionFinished();

public:
	UPROPERTY(Transient)
	bool bIsQueueing;

	UPROPERTY(Transient, BlueprintReadOnly)
	class AEMPCombatMapGameMode* OwningGameMode;
};
