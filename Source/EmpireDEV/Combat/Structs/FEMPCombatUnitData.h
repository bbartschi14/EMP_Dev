// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "FEMPCombatHitResult.h"
#include "FEMPCombatUnitData.generated.h"

UENUM(BlueprintType)
enum class EEMPNCORank : uint8
{
	NCO_NONE		UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class EEMPOfficerRank : uint8
{
	OR_NONE		UMETA(DisplayName = "None"),
	OR_LIEUTENANT		UMETA(DisplayName = "Lieutenant"),
};

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
	CC_PIKEMAN				UMETA(DisplayName = "Pikeman"),
	CC_WIZARD				UMETA(DisplayName = "Wizard")
};

/**
 * Class version of Combat Unit Data
 */
UCLASS(BlueprintType)
class EMPIREDEV_API UEMPCombatUnitData : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageTaken, FEMPCombatHitResult, hitResult);

	UPROPERTY(BlueprintAssignable)
		FOnDamageTaken OnDamageTaken;

	UEMPCombatUnitData();

	void InitializeDefaultSkills(const TArray<TSubclassOf<UEMPCombatSkill>>& InSkills);

	UFUNCTION(BlueprintCallable)
		void TakeHit(FEMPCombatHitResult hit)
	{
		CurrentHealth -= hit.DamageDealt;
		OnDamageTaken.Broadcast(hit);
	}

	UFUNCTION(BlueprintCallable)
		void SetDesiredLocation(FIntPoint newLocation)
	{
		DesiredLocationX = newLocation.X;
		DesiredLocationY = newLocation.Y;
	}

	UFUNCTION(BlueprintCallable)
		FIntPoint GetDesiredLocation() const
	{
		return FIntPoint(DesiredLocationX, DesiredLocationY);
	}

	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		FString CombatUnitName;

	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		int32 DesiredLocationX;

	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		int32 DesiredLocationY;

	// If null, this unit is currently unassigned
	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		class UEMPSquadData* OwningSquad;

	// Combat. Only used when on the combat map. This can allow us to serialize in-combat data too (:
	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		FIntPoint CombatLocation;

	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		int32 CurrentHealth;

	// Classes and Ranks
	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		EEMPCombatClass CombatClass;

	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		EEMPCombatClassRank CombatClassRank;

	// Officer status
	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		EEMPNCORank NCORank;

	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		EEMPOfficerRank OfficerRank;

	// Stats
	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		int32 Health;

	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		int32 Damage;

	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		int32 Armor;

	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		int32 Speed;

	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
		int32 Range;

	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
	bool bIsFriendlyUnit;

	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
	class AEMPCombatUnit* CombatUnitActor;

	/** Manages personal combat skills */
	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere)
	TArray<class UEMPCombatSkill*> CombatSkills;

	/** Filter combat skills by type without casting */
	UFUNCTION()
	void GetCombatSkillsOfType(TArray<class UEMPCombatSkill*>& OutSkills, FName InType);

	/** Get casted stat modifier skills */
	UFUNCTION(BlueprintCallable)
	void GetStatModifierCombatSkills(TArray<class UEMPStatModifierCombatSkill*>& OutSkills);
};

/**
 * Holds all the serialization/persistent data for a squad
 */
USTRUCT(BlueprintType)
struct FEMPCombatUnitDataStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UEMPCombatUnitData* GetCombatUnitData(class UGameInstanceBaseEMP* GameInstance) const;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CombatUnitName = "Default Name";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ClampMax = "4", UIMax = "4"))
		int32 DesiredLocationX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ClampMax = "4", UIMax = "4"))
		int32 DesiredLocationY;

	// Combat

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FIntPoint CombatLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CurrentHealth = 1;

	// Classes and Ranks

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEMPCombatClass CombatClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEMPCombatClassRank CombatClassRank;

	// Officer status

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEMPNCORank NCORank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEMPOfficerRank OfficerRank;
};