// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPCombatMapGrid.h"
#include "../Combat/Structs/FEMPCombatUnitData.h"
#include "../Combat/Structs/FEMPSquadData.h"
#include "../Combat/EMPCombatStatics.h"

void AEMPCombatMapGrid::BeginPlay()
{
	// Verify unique spawn points
	TSet<FIntPoint> spawnPointSet;
	for (FIntPoint point : FriendlySquadSpawnPoints)
	{
		spawnPointSet.Add(point);
	}
	for (FIntPoint point : EnemySquadSpawnPoints)
	{
		spawnPointSet.Add(point);
	}
	check(spawnPointSet.Num() == (FriendlySquadSpawnPoints.Num() + EnemySquadSpawnPoints.Num())); // If the number of items in the set is not equal to the number of spawn points, there was an overlap
	
	Super::BeginPlay();
}

void AEMPCombatMapGrid::SpawnFriendlyCombatUnit(UEMPCombatUnitData* friendlyCombatUnit)
{
	FEMPCombatUnitMeshData meshData;
	meshData.bIsFriendlyUnit = true;
	SpawnCombatUnit(friendlyCombatUnit, meshData);
}

void AEMPCombatMapGrid::SpawnEnemyCombatUnit(UEMPCombatUnitData* enemyCombatUnit)
{
	FEMPCombatUnitMeshData meshData;
	meshData.bIsFriendlyUnit = false;
	SpawnCombatUnit(enemyCombatUnit, meshData);
}

void AEMPCombatMapGrid::SpawnCombatUnit(UEMPCombatUnitData* combatUnitData, FEMPCombatUnitMeshData meshData)
{
	AEMPCombatUnit* combatUnit = GetWorld()->SpawnActor<AEMPCombatUnit>(CombatUnitClass);
	combatUnit->SetupCombatUnitMesh(meshData);
	combatUnit->InitializeCombatUnitData(combatUnitData);
	combatUnit->InitializeToGridSquare(combatUnitData->CombatLocation);
	combatUnit->TurnToDirection(combatUnitData->OwningSquad->CombatDirection);
	CombatUnits.Add(combatUnit);
}


TArray<FIntPoint> AEMPCombatMapGrid::GetFriendlySquadSpawnPoints() const
{
	return FriendlySquadSpawnPoints;
}

TArray<FIntPoint> AEMPCombatMapGrid::GetEnemySquadSpawnPoints() const
{
	return EnemySquadSpawnPoints;
}

void AEMPCombatMapGrid::TurnCombatUnitsInSquadToDirection(UEMPSquadData* squadData, EEMPCombatDirection direction)
{
	for (AEMPCombatUnit* unit : CombatUnits)
	{
		if (unit->GetCombatUnitData()->OwningSquad == squadData)
		{
			unit->TurnToDirection(direction);
		}
	}
}

void AEMPCombatMapGrid::TurnCombatUnitsInSquadToDefaultDirection(UEMPSquadData* squadData)
{
	for (AEMPCombatUnit* unit : CombatUnits)
	{
		if (unit->GetCombatUnitData()->OwningSquad == squadData)
		{
			unit->TurnToDirection(unit->GetCombatUnitData()->OwningSquad->CombatDirection);
		}
	}
}

void AEMPCombatMapGrid::SetCombatUnitsInSquadReady(UEMPSquadData* squadData, bool isReady)
{
	for (AEMPCombatUnit* unit : CombatUnits)
	{
		if (unit->GetCombatUnitData()->OwningSquad == squadData)
		{
			unit->SetReadyStance(isReady);
		}
	}
}