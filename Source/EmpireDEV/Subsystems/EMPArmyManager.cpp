// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPArmyManager.h"
#include "../Game/GameInstanceBaseEMP.h"
#include "Structs/EMPArmyDatabase.h"

void UEMPArmyManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UGameInstanceBaseEMP* gameInstance = Cast<UGameInstanceBaseEMP>(GetWorld()->GetGameInstance());
	if (gameInstance)
	{
		Resources = gameInstance->TestOnlyArmyDatabase->ArmyResources.GetArmyResources();
	}
}

void UEMPArmyManager::Deinitialize()
{
	Super::Deinitialize();
}

UEMPArmyResources* UEMPArmyManager::GetResources() const
{
	return Resources;
}
