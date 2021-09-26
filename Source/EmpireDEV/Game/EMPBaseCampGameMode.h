// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "BaseGameModeEMP.h"
#include "EMPBaseCampGameMode.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class EMPIREDEV_API AEMPBaseCampGameMode : public ABaseGameModeEMP
{
	GENERATED_BODY()

public:

#pragma region Delegates
		DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatUnitInteraction, class UEMPCombatUnitData*, combatUnitData);
		DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSquadSelected, class UEMPSquadData*, squadData);
		DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSimpleChange);

		/** Broadcasts the data of a combat unit when it is selected */
		UPROPERTY(BlueprintAssignable, Category = "EMP Events")
			FOnCombatUnitInteraction OnCombatUnitSelected;

		/** Broadcasts the data of a combat unit when it is hovered */
		UPROPERTY(BlueprintAssignable, Category = "EMP Events")
			FOnCombatUnitInteraction OnCombatUnitHovered;

		/** Called when the squad is selected */
		UPROPERTY(BlueprintAssignable, Category = "EMP Events")
			FOnSquadSelected OnSquadSelected;

		/** Called when the squad is deselected */
		UPROPERTY(BlueprintAssignable, Category = "EMP Events")
			FOnSimpleChange OnSquadDeselected;

		/** */
		UPROPERTY(BlueprintAssignable, Category = "EMP Events")
			FOnSimpleChange OnCombatUnitDeselected;

		/** */
		UPROPERTY(BlueprintAssignable, Category = "EMP Events")
			FOnSimpleChange OnCombatUnitUnhovered;

#pragma endregion Delegates

	/** Set a squad as the current selection, broadcasting to UI elements that will change state. */
	UFUNCTION(BlueprintCallable)
		void SelectSquad(class UEMPSquadData* squadToLoad);

	/** If a squad is currently selected, clear the reference and broadcast the deselection. */
	UFUNCTION(BlueprintCallable)
		void ClearSelectedSquad();

	UFUNCTION(BlueprintCallable)
		class UEMPSquadData* GetSelectedSquad() const;

	/** Set a combat unit as the current selection, broadcasting to UI elements that will change state. */
	UFUNCTION(BlueprintCallable)
		void SelectCombatUnit(class UEMPCombatUnitData* combatUnitToLoad);

	/** If a combat unit is currently selected, clear the reference and broadcast the deselection. */
	UFUNCTION(BlueprintCallable)
		void ClearSelectedCombatUnit();

	/** Set a combat unit as the current "hovered" unit, broadcasting to UI elements that will change state.  Hovering used for indicating the unit before selecting. */
	UFUNCTION(BlueprintCallable)
		void HoverCombatUnit(class UEMPCombatUnitData* combatUnitToLoad);

	/** "Unhover" the unit. Hovering used for indicating the unit before selecting. */
	UFUNCTION(BlueprintCallable)
		void UnhoverCombatUnit();


protected:
	virtual void BeginPlay() override;

	UPROPERTY(Transient)
		class UEMPSquadData* SelectedSquad;

	UPROPERTY(Transient)
		class UEMPCombatUnitData* SelectedCombatUnit;

	UPROPERTY(Transient)
		class UEMPCombatUnitData* HoveredCombatUnit;

private:
	UFUNCTION()
		void HandleSquadDissolved(class UEMPSquadData* squadDissolved);

	UFUNCTION()
		void HandleCombatUnitRemovedFromSquad(class UEMPCombatUnitData* combatUnitRemoved, class UEMPSquadData* squad);
};
