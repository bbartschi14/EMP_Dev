// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "EMPGridGameMode.h"
#include "EMPBetweenGameMenuMode.generated.h"

UENUM(BlueprintType)
enum class EBaseCampGameStateEMP : uint8
{
	BC_SELECTING_SQUAD     UMETA(DisplayName = "Selecting Squad"),
	BC_SELECTING_UNIT     UMETA(DisplayName = "Selecting Unit"),
	BC_EDITING_UNIT     UMETA(DisplayName = "Editing Unit"),
	BC_REARRANGING_SQUAD    UMETA(DisplayName = "Rearranging Squad"),
	BC_MOVING_UNIT    UMETA(DisplayName = "Moving Unit"),

};

/**
 * Used for the space between combat levels, where you can
 * manage your squads and do level selection. Sort of a "army camp"
 * or "home base" mode.
 */
UCLASS(Abstract, Blueprintable)
class EMPIREDEV_API AEMPBetweenGameMenuMode : public AEMPGridGameMode
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatUnitSelected, class UEMPCombatUnitData*, combatUnitData);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSquadSelected, class UEMPSquadData*, squadData);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EBaseCampGameStateEMP, NewState);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSimpleChange);

	/** Broadcasts the data of a combat unit when it is selected */
	UPROPERTY(BlueprintAssignable, Category = "EMP UI Events")
		FOnCombatUnitSelected OnCombatUnitSelected;

	/** Called when the game state is changed */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnGameStateChanged OnGameStateChanged;

	/** Called when the squad is selected */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnSquadSelected OnSquadSelected;

	/** Called when the squad is deselected */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnSimpleChange OnSquadDeselected;

	/** */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnSimpleChange OnCombatUnitDeselected;

	/** Loads a squad onto the preview grid, allowing for editing and maintenance */
	UFUNCTION()
		void SelectSquad(class UEMPSquadData* squadToLoad);

	UFUNCTION()
		void SelectCombatUnit(class UEMPCombatUnitData* combatUnitToLoad);

	UFUNCTION()
		void HandleCancelActionPressed();

	UFUNCTION(BlueprintCallable)
		void TryToEnterSquadRearranging();

	UFUNCTION(BlueprintCallable)
		void TryToExitSquadRearranging();

	UFUNCTION(BlueprintCallable)
		bool IsCombatUnitInSelectedSquad(class UEMPCombatUnitData* combatUnit);
protected:
	virtual void BeginPlay() override;

	// ~ Begin Grid Game Mode Overrides
	void HandleGridSquareClicked(class AEMPGridSquare* inGridSquare) override;

	void HandleSetGridHighlighted(class AEMPGridSquare* inGridSquare, bool isHighlighted) override;
	// ~ End Grid Game Mode Overrides

	UFUNCTION()
		bool IsGridConditionMetForGivenState(EBaseCampGameStateEMP givenState, class AEMPCombatUnit* unit);

	UPROPERTY(Transient)
		class UEMPSquadData* SelectedSquad;

	UPROPERTY(Transient)
		class UEMPCombatUnitData* SelectedCombatUnit;

	UPROPERTY(Transient)
		TArray<class AEMPCombatUnit*> CombatUnits;

	UFUNCTION()
		void ClearSelectedCombatUnit();

	UFUNCTION()
		void ClearSelectedSquad(bool bShouldBroadcast);

#pragma region Helpers
	UFUNCTION()
		bool IsLocationEmpty(FIntPoint location) const;

	UFUNCTION()
		class AEMPCombatUnit* GetCombatUnitAtLocation(FIntPoint location) const;
#pragma endregion Helpers

private:
	UFUNCTION()
		void SetCurrentGameState(EBaseCampGameStateEMP newGameState);

	/** Holds the current "state machine" style state */
	UPROPERTY()
		EBaseCampGameStateEMP CurrentGameState = EBaseCampGameStateEMP::BC_SELECTING_SQUAD;
};
