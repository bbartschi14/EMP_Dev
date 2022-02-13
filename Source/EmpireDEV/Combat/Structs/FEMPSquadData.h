// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "FEMPCombatUnitData.h"
#include "../Enums/EEMPCombatDirection.h"
#include "../../Game/GameInstanceBaseEMP.h"
#include "FEMPSquadData.generated.h"

/*
 * Squads start in the "awaiting orders" state at the start of a turn.
 */
UENUM(BlueprintType)
enum class ESquadStateEMP : uint8
{
	SS_AWAITING_ORDERS     UMETA(DisplayName = "Awaiting Orders"),
	SS_ACTION_QUEUED     UMETA(DisplayName = "Action queued"),
};

/**
 * Class version of Squad Data
 */
UCLASS(BlueprintType)
class EMPIREDEV_API UEMPSquadData : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable) UEMPCombatUnitData* GetCombatUnitAtDesiredLocation(FIntPoint desiredLocation) const;

	UFUNCTION(BlueprintCallable) UEMPCombatUnitData* GetCombatUnitAtCombatLocation(FIntPoint combatLocation) const;

	// Can return null if there is no officer
	UFUNCTION(BlueprintCallable) UEMPCombatUnitData* GetSquadOfficer() const;

	// Returns the current morale of the squad, calculated by combat unit data in the squad
	UFUNCTION(BlueprintCallable) int32 GetSquadMorale() const;

	// Get the squad composition as a map of classes (keys) to the number of units with that class (values)
	UFUNCTION(BlueprintCallable) TMap<EEMPCombatClass, int32> GetSquadComposition() const;

	UFUNCTION(BlueprintCallable) bool CanMoveToAreaCoordinate(FIntPoint areaCoordinate) const;

	UFUNCTION(BlueprintCallable) void HandleCombatUnitDied(UEMPCombatUnitData* deadCombatUnit);

	UFUNCTION(BlueprintCallable) void GetCombatActionSkills(TArray<class UEMPCombatActionSkill*>& OutSkills) const;

	UFUNCTION(BlueprintCallable) void GetCombatUnitsOfClass(EEMPCombatClass InClass, TArray<class UEMPCombatUnitData*>& OutUnits) const;

	UFUNCTION(BlueprintCallable) int32 GetNumberOfCombatUnitsOfClass(EEMPCombatClass InClass) const;
public:
	UPROPERTY(Transient, BlueprintReadWrite)
	FString SquadName;

	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<UEMPCombatUnitData*> CombatUnitsInSquad;

	// Combat

	UPROPERTY(Transient, BlueprintReadWrite)
	FIntPoint CombatAreaLocation;

	UPROPERTY(Transient, BlueprintReadWrite)
	EEMPCombatDirection CombatDirection;

	UPROPERTY(Transient, BlueprintReadWrite)
	ESquadStateEMP CurrentSquadState;

	UPROPERTY(Transient, BlueprintReadOnly)
	bool bIsFriendlySquad;

	/** Manages squad combat skills */
	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
	TArray<class UEMPCombatSkill*> CombatSkills;

	UPROPERTY(Transient, BlueprintReadOnly)
	class UEMPCombatActionSkill* QueuedAction;

};

/**
 * Holds all the serialized/persistent data for a squad
 */
USTRUCT(BlueprintType)
struct FEMPSquadDataStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UEMPSquadData* GetSquadData(UGameInstanceBaseEMP* GameInstance) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SquadName = "Default Name";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FEMPCombatUnitDataStruct> CombatUnitsInSquad;

	// Combat
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FIntPoint CombatAreaLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EEMPCombatDirection CombatDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ESquadStateEMP CurrentSquadState;
};
