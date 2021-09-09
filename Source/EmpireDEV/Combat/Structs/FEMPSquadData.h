// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "FEMPCombatUnitData.h"
#include "FEMPSquadData.generated.h"

/**
 * Class version of Squad Data
 */
UCLASS()
class EMPIREDEV_API UEMPSquadData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, BlueprintReadWrite)
		FString SquadName;

	UPROPERTY(Transient, BlueprintReadWrite)
		TArray<UEMPCombatUnitData*> CombatUnitsInSquad;
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
		for (FEMPCombatUnitDataStruct combatUnit : CombatUnitsInSquad)
		{
			squadData->CombatUnitsInSquad.Add(combatUnit.GetCombatUnitData());
		}
		return squadData;
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SquadName = "Default Name";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FEMPCombatUnitDataStruct> CombatUnitsInSquad;
};
