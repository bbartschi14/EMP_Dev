// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "BaseGameModeEMP.h"
#include "EMPCombatMapGameMode.generated.h"

UENUM(BlueprintType)
enum class EEMPCombatMapState : uint8
{
	GS_SELECTING_SQUAD     UMETA(DisplayName = "Selecting Squad"),
	GS_MOVING_SQUAD      UMETA(DisplayName = "Moving a Squad"),
	GS_REARRANGING_SQUAD      UMETA(DisplayName = "Rearranging a Squad"),
	GS_SIMULATING	UMETA(DisplayName = "Simulating"),
};

/**
* The game mode for controlling a basic game loop encounter.
* Two commanders fight against eachother, ordering their squadrons into
* positions to then simulate combat against the other team. The game mode
* ends when one team has eliminated the other, or one concedes.
***********************************************************************************/

UCLASS(Abstract, Blueprintable)
class EMPIREDEV_API AEMPCombatMapGameMode : public ABaseGameModeEMP
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EEMPCombatMapState, NewState);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSquadSelected, class UEMPSquadData*, squadData);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCombatEvent, class UEMPSquadData*, squadOne, class UEMPSquadData*, squadTwo);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatUnitChange, class UEMPCombatUnitData*, combatUnit);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCombatUnitAnimate, class UEMPCombatUnitData*, combatUnit, float, animationTime);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCombatUnitAttackAnimate, class UEMPCombatUnitData*, attackingCombatUnit, class UEMPCombatUnitData*, defendingCombatUnit, float, animationTime);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionQueued, class UEMPCombatActionData*, actionData);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMoraleRoll, class UEMPSquadData*, squadData, int32, moraleRoll, float, animationTime);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSimpleChange);

	/** */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnGameStateChanged OnGameStateChanged;

	/** Called when the squad is selected */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnSquadSelected OnSquadSelected;

	/** Called when the squad is deselected */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnSimpleChange OnSquadDeselected;

	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnCombatUnitChange OnCombatUnitDied;

	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnCombatUnitChange OnCombatUnitSelected;

	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnSimpleChange OnCombatUnitDeselected;

	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnCombatUnitAnimate OnCombatUnitMoved;

	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnCombatUnitAnimate OnCombatUnitMovementFailed;

	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnCombatUnitAttackAnimate OnCombatUnitAttackAnimate;

	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnCombatUnitAnimate OnCombatUnitRetreat;

	/** Called when a combat action is queued */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnActionQueued OnActionQueued;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "EMP Events")
		FOnActionQueued OnActionStarted;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "EMP Events")
		FOnActionQueued OnActionCompleted;

	/** Called when the first combat action in the queue is completed */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "EMP Events")
		FOnSimpleChange OnActionQueuePopped;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "EMP Events")
		FOnCombatEvent OnCombatInitiated;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "EMP Events")
		FOnCombatEvent OnCombatEnded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "EMP Events")
		FOnMoraleRoll OnMoraleRoll;

	/** Set a squad as the current selection, broadcasting to UI elements that will change state. */
	UFUNCTION(BlueprintCallable)
		void SelectSquad(class UEMPSquadData* squadToLoad);

	/** If a squad is currently selected, clear the reference and broadcast the deselection. */
	UFUNCTION(BlueprintCallable)
		void ClearSelectedSquad();

	/** Set a combat unit as the current selection, broadcasting to UI elements that will change state. */
	UFUNCTION(BlueprintCallable)
		void SelectCombatUnit(class UEMPCombatUnitData* combatUnitToLoad);

	UFUNCTION(BlueprintCallable)
		void ClearSelectedCombatUnit();

	UFUNCTION(BlueprintCallable)
		class UEMPSquadData* GetSelectedSquad() const;

	UFUNCTION(BlueprintCallable)
		class UEMPSquadData* GetFriendlySquadAtAreaCoordinate(FIntPoint areaCoordinate) const;

	UFUNCTION(BlueprintCallable)
		class UEMPSquadData* GetEnemySquadAtAreaCoordinate(FIntPoint areaCoordinate) const;

	UFUNCTION(BlueprintCallable)
		void EnterMovingSquadState();

	UFUNCTION(BlueprintCallable)
		void QueueAction_MoveSelectedSquad(FIntPoint newLocation);

	UFUNCTION(BlueprintCallable)
		void MoveSquadToAreaLocation(class UEMPSquadData* squadToMove, FIntPoint destination, float animationTime);

	UFUNCTION(BlueprintCallable)
		void MoveCombatUnitToLocation(class UEMPCombatUnitData* unitToMove, FIntPoint destination, float animationTime);

	UFUNCTION(BlueprintCallable)
		void RearrangeCombatUnitToLocation(class UEMPCombatUnitData* unitToMove, FIntPoint destination, float animationTime);

	UFUNCTION(BlueprintCallable)
		void HandleSquadMovementFailed(class UEMPSquadData* squadToMove, float animationTime);

	UFUNCTION(BlueprintCallable)
		void EnterRearrangingSquadState();

	UFUNCTION(BlueprintCallable)
		EEMPCombatMapState GetCombatMapState() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void EndTurn();

	UFUNCTION(BlueprintCallable)
		class UEMPCombatSimulator* InitiateCombat(class UEMPSquadData* squadOne, class UEMPSquadData* squadTwo);

	UFUNCTION(BlueprintCallable)
		void FinishSimulating();

	UFUNCTION()
		void RemoveSquadFromCombat(class UEMPSquadData* squadToRemove, float animationTime);

	UFUNCTION()
		void HandleCancelActionPressed();

protected:
	virtual void BeginPlay() override;

	/** Takes available squads and spawn locations, then sets area locations of squads and combat locations of units.
	 * Returns the squads that were successfully spawned.
	 */
	UFUNCTION()
		TArray<class UEMPSquadData*> SpawnSquads(TArray<class UEMPSquadData*> squads, TArray<FIntPoint> spawnLocations, bool bIsEnemySquad = false);

	UFUNCTION(BlueprintCallable)
		void SetCombatMapState(EEMPCombatMapState newState);

	UPROPERTY(Transient, BlueprintReadOnly)
		class AEMPCombatMapGrid* Grid;

	UPROPERTY(Transient, BlueprintReadOnly)
		TArray<class UEMPSquadData*> FriendlySquads;

	UPROPERTY(Transient, BlueprintReadOnly)
		TArray<class UEMPSquadData*> EnemySquads;

	UPROPERTY(Transient)
		class UEMPSquadData* SelectedSquad;

	UPROPERTY(Transient)
		class UEMPCombatUnitData* SelectedCombatUnit;

	UPROPERTY(Transient, BlueprintReadOnly)
		TArray<class UEMPCombatActionData*> QueuedActions;

	UPROPERTY(Transient, BlueprintReadWrite)
		class UEMPCombatSimulator* CombatSimulator;

	UPROPERTY(BlueprintReadOnly)
		EEMPCombatMapState CurrentState;

};
