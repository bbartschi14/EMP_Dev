// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPCombatActionSkill.h"
#include "../../Game/EMPCombatMapGameMode.h"
#include "../Structs/FEMPSquadData.h"

void UEMPCombatActionSkill::RequestActionQueue(AEMPCombatMapGameMode* InOwningGameMode)
{
	bIsQueueing = true;
	OwningGameMode = InOwningGameMode;
	OnActionRequestQueue();
}

void UEMPCombatActionSkill::ExecuteAction()
{
	OnCombatActionStartedExecuting.Broadcast(this);
	OnExecuteAction();
}

void UEMPCombatActionSkill::CancelQueue()
{
	bIsQueueing = false;
}

FName UEMPCombatActionSkill::GetSkillType() const
{
	return FName(TEXT("Action"));
}

void UEMPCombatActionSkill::ConfirmQueue()
{
	OwningSquad->QueuedAction = this;
	OwningGameMode->QueueAction(this);
	bIsQueueing = false;
}

void UEMPCombatActionSkill::OnExecutionFinished()
{
	OwningSquad->QueuedAction = nullptr;
	OnCombatActionFinishedExecuting.Broadcast(this);
}
