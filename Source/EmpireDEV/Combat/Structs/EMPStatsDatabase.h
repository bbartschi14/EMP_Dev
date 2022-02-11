// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EMPStatsDatabase.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FEMPDefaultStats
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Range;
};

/**
 *
 */
USTRUCT(BlueprintType)
struct FEMPDefaultStatsRankMap
{
	GENERATED_BODY()

public:
	FEMPDefaultStatsRankMap() 
	{
		RankToStatsMap.Add(EEMPCombatClassRank::CCR_RECRUIT, FEMPDefaultStats());
		RankToStatsMap.Add(EEMPCombatClassRank::CCR_COMMON, FEMPDefaultStats());
		RankToStatsMap.Add(EEMPCombatClassRank::CCR_EXPERIENCED, FEMPDefaultStats());
		RankToStatsMap.Add(EEMPCombatClassRank::CCR_VETERAN, FEMPDefaultStats());
		RankToStatsMap.Add(EEMPCombatClassRank::CCR_MASTER, FEMPDefaultStats());
	}
	UPROPERTY(EditAnywhere)
	TMap<EEMPCombatClassRank, FEMPDefaultStats> RankToStatsMap;
};

UCLASS(BlueprintType)
class EMPIREDEV_API UEMPStatsDatabase : public UDataAsset
{
	GENERATED_BODY()
public:
	UEMPStatsDatabase()
	{
		for (int i = 0; i < 8; i++)
			ClassToStatsMap.Add((EEMPCombatClass)i, FEMPDefaultStatsRankMap());
	}
	UPROPERTY(EditAnywhere)
	TMap<EEMPCombatClass, FEMPDefaultStatsRankMap> ClassToStatsMap;
};
