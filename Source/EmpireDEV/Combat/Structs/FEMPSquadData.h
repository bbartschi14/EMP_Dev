// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "FEMPCombatUnitData.h"
#include "../Enums/EEMPCombatDirection.h"
#include "FEMPSquadData.generated.h"


/*
 * Squads start in the "awaiting orders" state at the start of a turn.
 * When given a movement order, they will be in the "Move queued" state, meaning
 * that they can't be given another order or rearranged. If the squad is rearranged,
 * it will be put in the "Rearranged" state, and can be continuously rearranged but
 * not given a movement order.
 */
UENUM(BlueprintType)
enum class ESquadStateEMP : uint8
{
	SS_AWAITING_ORDERS     UMETA(DisplayName = "Awaiting Orders"),
	SS_MOVE_QUEUED     UMETA(DisplayName = "Move queued"),
	SS_REARRANGED     UMETA(DisplayName = "Rearranged"),
};


/**
 * Class version of Squad Data
 */
UCLASS(BlueprintType)
class EMPIREDEV_API UEMPSquadData : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable) UEMPCombatUnitData* GetCombatUnitAtDesiredLocation(FIntPoint desiredLocation)
	{
		for (UEMPCombatUnitData* combatUnit : CombatUnitsInSquad)
		{
			if (combatUnit->GetDesiredLocation() == desiredLocation)
			{
				return combatUnit;
			}
		}
		return nullptr;
	}

	UFUNCTION(BlueprintCallable) UEMPCombatUnitData* GetCombatUnitAtCombatLocation(FIntPoint combatLocation)
	{
		for (UEMPCombatUnitData* combatUnit : CombatUnitsInSquad)
		{
			if (combatUnit->CombatLocation == combatLocation)
			{
				return combatUnit;
			}
		}
		return nullptr;
	}

	// Can return null if there is no officer
	UFUNCTION(BlueprintCallable) UEMPCombatUnitData* GetSquadOfficer() const
	{
		for (UEMPCombatUnitData* combatUnit : CombatUnitsInSquad)
		{
			if (combatUnit->OfficerRank != EEMPOfficerRank::OR_NONE)
			{
				return combatUnit;
			}
		}
		return nullptr;
	}

	// Returns the current morale of the squad, calculated by combat unit data in the squad
	UFUNCTION(BlueprintCallable) int32 GetSquadMorale() const
	{
		int32 morale = 0;
		for (UEMPCombatUnitData* combatUnit : CombatUnitsInSquad)
		{
			morale += 5; // 5 base morale per unit 
			if (combatUnit->OfficerRank != EEMPOfficerRank::OR_NONE)
			{
				morale += 75; // 75 additional morale per officer
			}
			else if (combatUnit->NCORank != EEMPNCORank::NCO_NONE)
			{
				morale += 15; // 15 additional morale per NCO
			}
		}
		return morale;
	}

	UFUNCTION(BlueprintCallable) bool CanMoveToAreaCoordinate(FIntPoint areaCoordinate) const
	{
		int32 distance = FMath::Abs(CombatAreaLocation.X - areaCoordinate.X) + FMath::Abs(CombatAreaLocation.Y - areaCoordinate.Y);
		return distance == 1;
	}

	UFUNCTION(BlueprintCallable) void HandleCombatUnitDied(UEMPCombatUnitData* deadCombatUnit)
	{
		CombatUnitsInSquad.Remove(deadCombatUnit);
	}

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

};

/**
 * Holds all the serializable/persistent data for a squad
 */
USTRUCT(BlueprintType)
struct FEMPSquadDataStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UEMPSquadData* GetSquadData() const
	{
		UEMPSquadData* squadData = NewObject<UEMPSquadData>();
		squadData->SquadName = SquadName;
		squadData->CombatAreaLocation = CombatAreaLocation;
		squadData->CombatDirection = CombatDirection;
		squadData->CurrentSquadState = CurrentSquadState;
		for (FEMPCombatUnitDataStruct combatUnit : CombatUnitsInSquad)
		{
			UEMPCombatUnitData* combatUnitData = combatUnit.GetCombatUnitData();
			combatUnitData->OwningSquad = squadData;
			squadData->CombatUnitsInSquad.Add(combatUnitData);
		}
		return squadData;
	}

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
