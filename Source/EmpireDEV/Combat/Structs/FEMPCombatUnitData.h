// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "FEMPCombatUnitData.generated.h"

UENUM(BlueprintType)
enum class EEMPCombatClassRank : uint8
{
	CCR_RECRUIT			UMETA(DisplayName = "Recruit"),
	CCR_COMMON			UMETA(DisplayName = "Common"),
	CCR_EXPERIENCED		UMETA(DisplayName = "Experienced"),
	CCR_VETERAN			UMETA(DisplayName = "Veteran"),
	CCR_MASTER			UMETA(DisplayName = "Master")
};

UENUM(BlueprintType)
enum class EEMPCombatClass : uint8
{
	CC_LIGHT_INFANTRY		UMETA(DisplayName = "Light Infantry"),
	CC_HEAVY_INFANTRY		UMETA(DisplayName = "Heavy Infantry"),
	CC_LIGHT_CAVALRY		UMETA(DisplayName = "Light Cavalry"),
	CC_HEAVY_CAVALRY		UMETA(DisplayName = "Heavy Cavalry"),
	CC_ARCHER				UMETA(DisplayName = "Archer"),
	CC_ENGINEER				UMETA(DisplayName = "Engineer"),
	CC_SCOUT				UMETA(DisplayName = "Scout"),
	CC_PIKEMAN				UMETA(DisplayName = "Pikeman")
};

/**
 * Class version of Combat Unit Data
 */
UCLASS(BlueprintType)
class EMPIREDEV_API UEMPCombatUnitData : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void SetDesiredLocation(FIntPoint newLocation)
	{
		DesiredLocationX = newLocation.X;
		DesiredLocationY = newLocation.Y;
	}

	UFUNCTION()
		FIntPoint GetDesiredLocation()
	{
		return FIntPoint(DesiredLocationX, DesiredLocationY);
	}

	UPROPERTY(Transient, BlueprintReadWrite)
		FString CombatUnitName;

	UPROPERTY(Transient, BlueprintReadWrite)
		int32 DesiredLocationX;

	UPROPERTY(Transient, BlueprintReadWrite)
		int32 DesiredLocationY;

	// If null, this unit is currently unassigned
	UPROPERTY(Transient, BlueprintReadWrite)
		class UEMPSquadData* OwningSquad;

	// Classes and Ranks
	
	UPROPERTY(Transient, BlueprintReadWrite)
		EEMPCombatClass CombatClass;

	UPROPERTY(Transient, BlueprintReadWrite)
		EEMPCombatClassRank CombatClassRank;

	// Stats

	UPROPERTY(Transient, BlueprintReadWrite)
		int32 Health;

	UPROPERTY(Transient, BlueprintReadWrite)
		int32 Damage;

	UPROPERTY(Transient, BlueprintReadWrite)
		int32 Armor;

	UPROPERTY(Transient, BlueprintReadWrite)
		int32 Speed;

	UPROPERTY(Transient, BlueprintReadWrite)
		int32 Range;
};

/**
 * Holds all the serializable/persistent data for a squad
 */
USTRUCT(BlueprintType)
struct FEMPCombatUnitDataStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UEMPCombatUnitData* GetCombatUnitData() const
	{
		UEMPCombatUnitData* combatUnitData = NewObject<UEMPCombatUnitData>();

		combatUnitData->CombatUnitName = CombatUnitName;
		combatUnitData->DesiredLocationX = DesiredLocationX;
		combatUnitData->DesiredLocationY = DesiredLocationY;

		// Classes and Ranks

		combatUnitData->CombatClass = CombatClass;
		combatUnitData->CombatClassRank = CombatClassRank;

		// Stats
		combatUnitData->Health = Health;
		combatUnitData->Damage = Damage;
		combatUnitData->Armor = Armor;
		combatUnitData->Speed = Speed;
		combatUnitData->Range = Range;

		return combatUnitData;
	}

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CombatUnitName = "Default Name";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ClampMax = "4", UIMax = "4"))
		int32 DesiredLocationX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ClampMax = "4", UIMax = "4"))
		int32 DesiredLocationY;

	// Classes and Ranks

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEMPCombatClass CombatClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEMPCombatClassRank CombatClassRank;

	// Stats

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Health = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Damage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Armor = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Speed = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Range = 1;
};