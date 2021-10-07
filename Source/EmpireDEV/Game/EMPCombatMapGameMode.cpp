// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPCombatMapGameMode.h"
#include <Kismet/GameplayStatics.h>
#include "../Level/EMPCombatMapGrid.h"
#include "../Combat/Structs/FEMPSquadData.h"
#include "../Combat/EMPCombatPlayer.h"
#include "../Subsystems/EMPSquadManager.h"
#include "../Combat/EMPEnemySquadLevelData.h"
#include "../Combat/EMPCombatStatics.h"
#include "../Combat/CombatActions/EMPMovementActionData.h"
#include "../Combat/EMPCombatSimulator.h"

void AEMPCombatMapGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Get Combat Map Grid
	TArray<AActor*> FoundCombatMapGrids;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEMPCombatMapGrid::StaticClass(), FoundCombatMapGrids);
	check(FoundCombatMapGrids.Num() > 0); // Make sure there is combat map grid on this level
	Grid = Cast<AEMPCombatMapGrid>(FoundCombatMapGrids[0]);

	// Get Enemy Squad Data
	TArray<AActor*> FoundEnemySquadDatas;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEMPEnemySquadLevelData::StaticClass(), FoundEnemySquadDatas);
	check(FoundEnemySquadDatas.Num() > 0); // Make sure there is enemy squad data on this level
	AEMPEnemySquadLevelData* enemySquadData = Cast<AEMPEnemySquadLevelData>(FoundEnemySquadDatas[0]);

	// Setup squads in combat
	UEMPSquadManager* squadManager = GetWorld()->GetGameInstance()->GetSubsystem<UEMPSquadManager>();
	check(squadManager);

	FriendlySquads = SpawnSquads(squadManager->GetSquadData(), Grid->GetFriendlySquadSpawnPoints());
	EnemySquads = SpawnSquads(enemySquadData->GetSquadData(), Grid->GetEnemySquadSpawnPoints(), true);
}

TArray<UEMPSquadData*> AEMPCombatMapGameMode::SpawnSquads(TArray<UEMPSquadData*> squads, TArray<FIntPoint> spawnLocations, bool bIsEnemySquad)
{
	TArray<UEMPSquadData*> squadsSpawned;

	for (int i = 0; i < spawnLocations.Num(); i++)
	{
		if (squads.IsValidIndex(i))
		{
			FIntPoint spawnLocation = spawnLocations[i];

			// Spawn squad at spawn location
			UEMPSquadData* squad = squads[i];

			squad->CombatAreaLocation = spawnLocation;
			squad->CombatDirection = bIsEnemySquad ? EEMPCombatDirection::CD_DOWN : EEMPCombatDirection::CD_UP;
			for (UEMPCombatUnitData* combatUnit : squad->CombatUnitsInSquad)
			{
				// Rotating units into starting direction
				FIntPoint rotatedLocalLocation = UEMPCombatStatics::RotateLocalGridCoordinate(combatUnit->GetDesiredLocation(), squad->CombatDirection);
				combatUnit->CombatLocation = UEMPCombatStatics::TransformGridCoordinate_LocalToGlobal(rotatedLocalLocation, squad->CombatAreaLocation);
				combatUnit->CurrentHealth = combatUnit->Health; // Resetting health here on spawn. Maybe could be moved somewhere else.
				if (bIsEnemySquad)
				{
					combatUnit->bIsFriendlyUnit = false;
					Grid->SpawnEnemyCombatUnit(combatUnit);
				}
				else
				{
					combatUnit->bIsFriendlyUnit = true;
					Grid->SpawnFriendlyCombatUnit(combatUnit);
				}
			}

			squadsSpawned.Add(squad);
		}
		else
		{
			// Once we run out of squads to fill our spawn locations, we can just exit.
			break;
		}
	}

	return squadsSpawned;
}

void AEMPCombatMapGameMode::SelectSquad(UEMPSquadData* squadToLoad)
{
	// Start by clearing previous selection
	ClearSelectedSquad();

	// Assign reference and broadcast
	SelectedSquad = squadToLoad;
	OnSquadSelected.Broadcast(SelectedSquad);
}

void AEMPCombatMapGameMode::ClearSelectedSquad()
{
	ClearSelectedCombatUnit();
	if (SelectedSquad)
	{
		SelectedSquad = nullptr;
		OnSquadDeselected.Broadcast();
	}
}

void AEMPCombatMapGameMode::SelectCombatUnit(UEMPCombatUnitData* combatUnitToLoad)
{
	ClearSelectedCombatUnit();
	SelectedCombatUnit = combatUnitToLoad;
	OnCombatUnitSelected.Broadcast(SelectedCombatUnit);
}

void AEMPCombatMapGameMode::ClearSelectedCombatUnit()
{
	if (SelectedCombatUnit)
	{
		SelectedCombatUnit = nullptr;
		OnCombatUnitDeselected.Broadcast();
	}
}

UEMPSquadData* AEMPCombatMapGameMode::GetSelectedSquad() const
{
	return SelectedSquad;
}

UEMPSquadData* AEMPCombatMapGameMode::GetFriendlySquadAtAreaCoordinate(FIntPoint areaCoordinate) const
{
	for (UEMPSquadData* squad : FriendlySquads)
	{
		if (squad->CombatAreaLocation == areaCoordinate)
		{
			return squad;
		}
	}
	return nullptr;
}

UEMPSquadData* AEMPCombatMapGameMode::GetEnemySquadAtAreaCoordinate(FIntPoint areaCoordinate) const
{
	for (UEMPSquadData* squad : EnemySquads)
	{
		if (squad->CombatAreaLocation == areaCoordinate)
		{
			return squad;
		}
	}
	return nullptr;
}

void AEMPCombatMapGameMode::EnterMovingSquadState()
{
	if (SelectedSquad)
	{
		SetCombatMapState(EEMPCombatMapState::GS_MOVING_SQUAD);
	}
}

void AEMPCombatMapGameMode::QueueAction_MoveSelectedSquad(FIntPoint newLocation)
{
	check(SelectedSquad && SelectedSquad->CanMoveToAreaCoordinate(newLocation));

	SelectedSquad->CurrentSquadState = ESquadStateEMP::SS_MOVE_QUEUED;
	UEMPMovementActionData* actionData = NewObject<UEMPMovementActionData>();
	actionData->SquadToMove = SelectedSquad;
	actionData->Destination = newLocation;

	QueuedActions.Add(actionData);
	OnActionQueued.Broadcast(actionData);

	ClearSelectedSquad();

	SetCombatMapState(EEMPCombatMapState::GS_SELECTING_SQUAD);
}

void AEMPCombatMapGameMode::MoveSquadToAreaLocation(UEMPSquadData* squadToMove, FIntPoint destination, float animationTime)
{
	FIntPoint areaCoordinateDelta = destination - squadToMove->CombatAreaLocation;
	squadToMove->CombatAreaLocation = destination;
	
	for (UEMPCombatUnitData* combatUnit : squadToMove->CombatUnitsInSquad)
	{
		FIntPoint currentGridCoordinate = combatUnit->CombatLocation;
		FIntPoint destinationGridCoordinate = FIntPoint(currentGridCoordinate.X + areaCoordinateDelta.X * 5, currentGridCoordinate.Y + areaCoordinateDelta.Y * 5);
		MoveCombatUnitToLocation(combatUnit, destinationGridCoordinate, animationTime);
	}
}

void AEMPCombatMapGameMode::MoveCombatUnitToLocation(UEMPCombatUnitData* unitToMove, FIntPoint destination, float animationTime)
{
	unitToMove->CombatLocation = destination;
	OnCombatUnitMoved.Broadcast(unitToMove, animationTime);
}

void AEMPCombatMapGameMode::RearrangeCombatUnitToLocation(UEMPCombatUnitData* unitToMove, FIntPoint destination, float animationTime)
{
	if (UEMPCombatStatics::IsGridCoordinateInArea(destination, unitToMove->OwningSquad->CombatAreaLocation))
	{
		UEMPCombatUnitData* potentialUnitAtDestination = SelectedSquad->GetCombatUnitAtCombatLocation(destination);
		if (potentialUnitAtDestination)
		{
			MoveCombatUnitToLocation(potentialUnitAtDestination, unitToMove->CombatLocation, animationTime);
		}
		MoveCombatUnitToLocation(unitToMove, destination, animationTime);
		SelectedSquad->CurrentSquadState = ESquadStateEMP::SS_REARRANGED;
	}
}

void AEMPCombatMapGameMode::HandleSquadMovementFailed(UEMPSquadData* squadToMove, float animationTime)
{
	for (UEMPCombatUnitData* combatUnit : squadToMove->CombatUnitsInSquad)
	{
		OnCombatUnitMovementFailed.Broadcast(combatUnit, animationTime);
	}
}

void AEMPCombatMapGameMode::EnterRearrangingSquadState()
{
	if (SelectedSquad)
	{
		SetCombatMapState(EEMPCombatMapState::GS_REARRANGING_SQUAD);
	}
}

EEMPCombatMapState AEMPCombatMapGameMode::GetCombatMapState() const
{
	return CurrentState;
}

void AEMPCombatMapGameMode::SetCombatMapState(EEMPCombatMapState newState)
{
	OnExitGameState.Broadcast(CurrentState);
	CurrentState = newState;
	OnGameStateChanged.Broadcast(newState);
}

UEMPCombatSimulator* AEMPCombatMapGameMode::InitiateCombat(UEMPSquadData* squadOne, UEMPSquadData* squadTwo)
{
	UEMPCombatSimulator* simulator = NewObject<UEMPCombatSimulator>();
	simulator->InitializeCombatData(squadOne, squadTwo, this);

	OnCombatInitiated.Broadcast(squadOne, squadTwo);

	return simulator;
}



void AEMPCombatMapGameMode::FinishSimulating()
{
	// Cleanup and reset relevant data
	for (UEMPSquadData* squad : FriendlySquads)
	{
		squad->CurrentSquadState = ESquadStateEMP::SS_AWAITING_ORDERS;
	}

	SetCombatMapState(EEMPCombatMapState::GS_SELECTING_SQUAD);
}

void AEMPCombatMapGameMode::RemoveSquadFromCombat(UEMPSquadData* squadToRemove, float animationTime)
{
	if (FriendlySquads.Contains(squadToRemove))
	{
		FriendlySquads.Remove(squadToRemove);
	}
	else if (EnemySquads.Contains(squadToRemove))
	{
		EnemySquads.Remove(squadToRemove);
	}

	for (UEMPCombatUnitData* combatUnit : squadToRemove->CombatUnitsInSquad)
	{
		OnCombatUnitRetreat.Broadcast(combatUnit, animationTime);
	}
}

void AEMPCombatMapGameMode::HandleCancelActionPressed()
{
	switch (CurrentState)
	{
	case EEMPCombatMapState::GS_SELECTING_SQUAD:
		if (SelectedSquad)
		{
			ClearSelectedSquad();
		}
		break;
	case EEMPCombatMapState::GS_MOVING_SQUAD:
		ClearSelectedSquad();
		SetCombatMapState(EEMPCombatMapState::GS_SELECTING_SQUAD);
		break;
	case EEMPCombatMapState::GS_REARRANGING_SQUAD:
		ClearSelectedSquad();
		SetCombatMapState(EEMPCombatMapState::GS_SELECTING_SQUAD);
		break;
	}
}