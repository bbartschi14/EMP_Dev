// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPEnemySquadLevelData.h"
#include "../Game/GameInstanceBaseEMP.h"

AEMPEnemySquadLevelData::AEMPEnemySquadLevelData()
{
	PrimaryActorTick.bCanEverTick = false;
}

TArray<UEMPSquadData*> AEMPEnemySquadLevelData::GetSquadData() const
{
	UGameInstanceBaseEMP* gameInstance = Cast<UGameInstanceBaseEMP>(GetWorld()->GetGameInstance());

	TArray<UEMPSquadData*> squadData;
	for (FEMPSquadDataStruct data : EnemySquads)
	{
		UEMPSquadData* squad = data.GetSquadData(gameInstance);
		squad->bIsFriendlySquad = false;
		squadData.Add(squad);
	}
	return squadData;
}

void AEMPEnemySquadLevelData::BeginPlay()
{
	Super::BeginPlay();
}


