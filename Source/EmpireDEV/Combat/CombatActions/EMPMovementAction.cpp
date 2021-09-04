// Empire 2021. Ben Bartschi and Jacob Bartschi

#include "EMPMovementAction.h"
#include "../../Combat/EMPSquad.h"
#include "../../Game/BasicLevelGameModeEMP.h"

void UEMPMovementAction::ExecuteAction(ABasicLevelGameModeEMP* owningGameMode)
{
	Super::ExecuteAction(owningGameMode);
	AEMPSquad* friendlySquad = owningGameMode->GetFriendlySquadAtAreaCoordinate(DestinationAreaCoordinate);
	AEMPSquad* enemySquad = owningGameMode->GetEnemySquadAtAreaCoordinate(DestinationAreaCoordinate);

	if (enemySquad)
	{
		// Handle combat
		owningGameMode->InitializeCombat(SquadToMove, enemySquad);
	}
	else if (friendlySquad)
	{
		// Can't move onto same square as other friendly squad
		SquadToMove->HandleMovementFailed();
	}
	else
	{
		// Successful move
		SquadToMove->MoveToAreaCoordinate(DestinationAreaCoordinate);
	}
}

void UEMPMovementAction::SetSquad(AEMPSquad* squadToMove)
{
	SquadToMove = squadToMove;
}

void UEMPMovementAction::SetDestinationAreaCoordinate(FIntPoint areaCoordinate)
{
	DestinationAreaCoordinate = areaCoordinate;
}
