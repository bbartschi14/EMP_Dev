// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "BaseGameModeEMP.h"
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
class EMPIREDEV_API ABasicLevelGameModeEMP : public ABaseGameModeEMP
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

	UFUNCTION(BlueprintCallable)
	EBasicLevelGameStateEMP GetCurrentGameState() const;

	void SetCurrentGameState(EBasicLevelGameStateEMP newState);

	void HandleCancelActionPressed();

	UFUNCTION()
		class AEMPSquad* GetFriendlySquadAtAreaCoordinate(FIntPoint areaCoordinate);

	UFUNCTION()
		class AEMPSquad* GetEnemySquadAtAreaCoordinate(FIntPoint areaCoordinate);

protected:
	virtual void BeginPlay() override;

#pragma region Grid
	/** Create grid squares from member properties */
	UFUNCTION()
	void SpawnGrid();

	UFUNCTION(BlueprintCallable)
	FVector GetWorldLocationFromGridLocation(FIntPoint gridLocation);

	UFUNCTION()
	void HandleGridSquareClicked(class AEMPGridSquare* inGridSquare);

	UFUNCTION()
	void HandleGridSquareHovered(class AEMPGridSquare* inGridSquare);

	UFUNCTION()
	void HandleGridSquareUnhovered(class AEMPGridSquare* inGridSquare);

	UFUNCTION()
		void HandleSetGridHighlighted(class AEMPGridSquare* inGridSquare, bool isHighlighted);

	UFUNCTION()
		void HandleSetGridAreaHighlighted(FIntPoint areaCoordinate, bool isHighlighted);

	UFUNCTION()
		bool IsGridCoordinateInAreaCoordinate(FIntPoint gridCoordinate, FIntPoint areaCoordinate);

	UFUNCTION()
		FGridDataEMP GetGridDataForSquare(class AEMPGridSquare* inGridSquare);

	/** Helper function for getting the grid square in 1D array from 2D coordinate */
	UFUNCTION()
		AEMPGridSquare* GetGridSquareAtCoordinate(FIntPoint gridCoordinate) const;

	/** An area coordinate is the coordinate of the 5x5 section. I.e., the first 5 grid squares in the first 5 columns is in area (0, 0) */
	UFUNCTION()
	FIntPoint GetAreaCoordinateOfGridCoordinate(FIntPoint gridCoordinate) const;

	UFUNCTION()
		TArray<class AEMPGridSquare*> GetGridSquaresInArea(FIntPoint areaCoordinate) const;

	UPROPERTY(Transient)
		TArray<class AEMPGridSquare*> GridSquares;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AEMPGridSquare> GridSquareClass;

	UPROPERTY(EditDefaultsOnly)
		FIntPoint GridDimensions;

	UPROPERTY(EditDefaultsOnly)
		FVector2D SingleGridSquareSize;

	UPROPERTY(EditDefaultsOnly)
		float GridBaseHeight;

	UPROPERTY()
		int32 GridAreaSize = 5;

	UPROPERTY()
		FIntPoint GridAreaEditing;

	UPROPERTY(Transient)
		class AEMPGridSquare* HoveredGridSquare;

#pragma endregion Grid

#pragma region GridStateConditions

	UFUNCTION()
		bool IsGridConditionMetForGivenState(EBasicLevelGameStateEMP givenState, FGridDataEMP gridData);

#pragma endregion GridStateConditions

#pragma region Combat
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AEMPCombatUnit> CombatUnitClass;

	UPROPERTY(Transient)
		class AEMPCombatPlayer* FriendlyCombatPlayer;

	UPROPERTY(Transient)
		class AEMPCombatPlayer* EnemyCombatPlayer;

	UPROPERTY(Transient)
		class AEMPSquad* SelectedSquad;

	UPROPERTY(Transient)
		class AEMPCombatUnit* SelectedUnit;

	UPROPERTY()
		TArray<class UEMPCombatAction*> CombatActionsQueue;

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
#pragma endregion Combat

private:

	/** Holds the current "state machine" style state */
	EBasicLevelGameStateEMP CurrentGameState = EBasicLevelGameStateEMP::GS_SELECTING_SQUAD;
};
