// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPArmyManager.h"
#include "../Game/GameInstanceBaseEMP.h"
#include "Structs/EMPArmyDatabase.h"
#include "EMPSquadManager.h"

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

void UEMPArmyManager::GiveMoney(int32 Amount)
{
	Resources->Money += Amount;
	OnArmyResourcesChanged.Broadcast();
}

void UEMPArmyManager::RecruitUnit(FEMPCombatUnitDataStruct Unit, int32 Cost)
{
	if (Resources->Money >= Cost)
	{
		SubtractMoney(Cost);
		GetGameInstance()->GetSubsystem<UEMPSquadManager>()->CreateUnassignedUnit(Unit);
	}
}

void UEMPArmyManager::SubtractMoney(int32 Amount)
{
	Resources->Money -= Amount;
	OnArmyResourcesChanged.Broadcast();
}
