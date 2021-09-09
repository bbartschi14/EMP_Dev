// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "EMPGridGameMode.h"
#include "BasicLevelGameModeEMP.generated.h"

UENUM(BlueprintType)
enum class EBasicLevelGameStateEMP : uint8
{
	GS_SELECTING_SQUAD     UMETA(DisplayName = "Selecting Squad"),
	GS_ORDERING_SQUAD      UMETA(DisplayName = "Ordering a Squad"),
	GS_MOVING_SQUAD      UMETA(DisplayName = "Moving a Squad"),
	GS_REARRANGING_SQUAD      UMETA(DisplayName = "Rearranging a Squad"),
	GS_MOVING_UNIT      UMETA(DisplayName = "Moving a Unit"),
	GS_RESOLVING_ORDERS   UMETA(DisplayName = "Resolving Orders"),
	GS_RESOLVING_COMBAT   UMETA(DisplayName = "Resolving Combat"),
	GS_RESET_SQUAD_AFTER_COMBAT   UMETA(DisplayName = "Reset Squad After Combat"),

};

USTRUCT() 
struct FGridDataEMP
{
	GENERATED_BODY()

public:
	UPROPERTY()
		FIntPoint gridCoordinate;

	UPROPERTY()
		FIntPoint areaCoordinate;

	UPROPERTY()
		class AEMPCombatUnit* unitAtCoordinate;

	UPROPERTY()
		class AEMPSquad* squadAtArea;
};

/**
* The game mode for controlling a basic game loop encounter.
* Two commanders fight against eachother, ordering their squadrons into
* positions to then simulate combat against the other team. The game mode
* ends when one team has eliminated the other, or one concedes.
***********************************************************************************/

UCLASS(Abstract, Blueprintable)
class EMPIREDEV_API ABasicLevelGameModeEMP : public AEMPGridGameMode
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EBasicLevelGameStateEMP, NewState);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSquadSelected, class AEMPSquad*, SelectedSquad);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionQueued, FString, ActionDescription);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSimpleChange);

	/** Called when the game state is changed */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnGameStateChanged OnGameStateChanged;

	/** Called when a squad is selected */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnSquadSelected OnSquadSelected;

	/** Called when an action is queued */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnActionQueued OnActionQueued;

	/** Called when the first action in the queue is finished */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnSimpleChange OnActionQueuePopped;

	/** Called when the selected squad is deselected */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnSimpleChange OnSquadDeselected;

	UPROPERTY(EditDefaultsOnly)
		bool bEnableDebugMode;

	UFUNCTION(BlueprintCallable)
	EBasicLevelGameStateEMP GetCurrentGameState() const;

	void SetCurrentGameState(EBasicLevelGameStateEMP newState);

	void HandleCancelActionPressed();

	UFUNCTION()
		class AEMPSquad* GetFriendlySquadAtAreaCoordinate(FIntPoint areaCoordinate);

	UFUNCTION()
		class AEMPSquad* GetEnemySquadAtAreaCoordinate(FIntPoint areaCoordinate);

	/** Initialize combat between two squad */
	UFUNCTION()
		void InitializeCombat(class AEMPSquad* squadOne, class AEMPSquad* squadTwo);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
		FIntPoint GridAreaEditing;

	UFUNCTION()
		FGridDataEMP GetGridDataForSquare(class AEMPGridSquare* inGridSquare);

	// ~ Begin Grid Game Mode Overrides
	void HandleGridSquareClicked(class AEMPGridSquare* inGridSquare) override;

	void HandleSetGridHighlighted(class AEMPGridSquare* inGridSquare, bool isHighlighted) override;
	// ~ End Grid Game Mode Overrides

#pragma region GridStateConditions

	UFUNCTION()
		bool IsGridConditionMetForGivenState(EBasicLevelGameStateEMP givenState, FGridDataEMP gridData);

#pragma endregion GridStateConditions

#pragma region Combat

	UPROPERTY(Transient, BlueprintReadOnly)
		class AEMPCombatPlayer* FriendlyCombatPlayer;

	UPROPERTY(Transient, BlueprintReadOnly)
		class AEMPCombatPlayer* EnemyCombatPlayer;

	UPROPERTY(Transient)
		class AEMPSquad* SelectedSquad;

	UPROPERTY(Transient)
		class AEMPCombatUnit* SelectedUnit;

	UPROPERTY(Transient)
		class AEMPSquad* InCombatSquadOne;

	UPROPERTY(Transient)
		class AEMPSquad* InCombatSquadTwo;

	UPROPERTY(Transient)
		int32 CurrentCombatRound;

	UPROPERTY()
		TArray<class UEMPCombatAction*> CombatActionsQueue;

	UFUNCTION(BlueprintCallable)
		void LoadSquad(class AEMPCombatPlayer* owningCombatPlayer, class UEMPSquadData* squadData, FIntPoint areaCoordinate);

	/** Allow squad setup in blueprints */
	UFUNCTION(BlueprintImplementableEvent)
		void LoadFriendlySquads(class AEMPCombatPlayer* friendlyPlayer);

	/** Allow squad setup in blueprints */
	UFUNCTION(BlueprintImplementableEvent)
		void LoadEnemySquads(class AEMPCombatPlayer* enemyPlayer);

	UFUNCTION()
		void SelectSquad(class AEMPSquad* squadToSelect);

	UFUNCTION()
		void DeselectSquad();

	UFUNCTION()
		void SelectUnit(class AEMPCombatUnit* unitToSelect);

	UFUNCTION()
		void DeselectUnit();

	UFUNCTION(BlueprintCallable)
		void EnterRearrangingSquadState();

	UFUNCTION(BlueprintCallable)
		void EnterMovingSquadState();

	UFUNCTION()
		void QueueAction_MoveSquad(class AEMPSquad* squadToMove, FIntPoint areaCoordinateOfDestination);

	UFUNCTION(BlueprintCallable)
		void EnterResolvingOrdersState();

	UFUNCTION(BlueprintCallable)
		void ResolveCurrentCombatAction();

	UFUNCTION(BlueprintCallable)
		void HandleCombatUnitAnimationFinished();
	
	UFUNCTION()
		void TryStartCombatRound();

	UFUNCTION()
		void EndCombat();

	UFUNCTION()
		void ResetSquadAfterCombat();

	/** Simulate damage round of combat */
	UFUNCTION()
		void SimulateCombatDamage(class AEMPSquad* squadOne, class AEMPSquad* squadTwo);

	/** Update positions of combat units (typically after damage phase). Returns true if any units move */
	UFUNCTION()
		bool UpdateCombatMovement(class AEMPSquad* squadOne, class AEMPSquad* squadTwo);

	/** Simulate squad attacks */
	UFUNCTION()
		void SimulateSquadAttacks(class AEMPSquad* attackingSquad, class AEMPSquad* defendingSquad, FIntPoint attackingDirection);

#pragma endregion Combat

#pragma region MathHelpers
	/** */
	UFUNCTION()
		FIntPoint GetPerpendicularOfIntPoint_Clockwise(FIntPoint inIntPoint);

	/** */
	UFUNCTION()
		FIntPoint GetPerpendicularOfIntPoint_CounterClockwise(FIntPoint inIntPoint);
#pragma endregion MathHelpers

#pragma region GameStateControls

#pragma endregion GameStateControls

private:

	/** Holds the current "state machine" style state */
	UPROPERTY()
	EBasicLevelGameStateEMP CurrentGameState = EBasicLevelGameStateEMP::GS_SELECTING_SQUAD;
};
