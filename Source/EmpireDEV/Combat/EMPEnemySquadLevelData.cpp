// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPEnemySquadLevelData.h"

AEMPEnemySquadLevelData::AEMPEnemySquadLevelData()
{
	PrimaryActorTick.bCanEverTick = false;
}

TArray<UEMPSquadData*> AEMPEnemySquadLevelData::GetSquadData() const
{
	TArray<UEMPSquadData*> squadData;
	for (FEMPSquadDataStruct data : EnemySquads)
	{
		squadData.Add(data.GetSquadData());
	}
	return squadData;
}

void AEMPEnemySquadLevelData::BeginPlay()
{
	Super::BeginPlay();
}


