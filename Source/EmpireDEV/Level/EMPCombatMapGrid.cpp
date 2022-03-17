// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPCombatMapGrid.h"
#include "../Combat/Structs/FEMPCombatUnitData.h"
#include "../Combat/Structs/FEMPSquadData.h"
#include "../Combat/EMPCombatStatics.h"
#include "../Combat/Data/CombatMapDataAsset.h"
#include "../Combat/EMPCombatSquad.h"
#include "../Level/EMPGridSquare.h"

void AEMPCombatMapGrid::BeginPlay()
{
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
	combatUnit->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	combatUnit->SetupCombatUnitMesh(meshData);
	combatUnit->InitializeCombatUnitData(combatUnitData);
	combatUnit->InitializeToGridSquare(combatUnitData->CombatLocation, .25f);
	combatUnit->TurnToDirection(combatUnitData->OwningSquad->CombatDirection);
	CombatUnits.Add(combatUnit);
}

void AEMPCombatMapGrid::MapInitialize()
{
	LoadMapFromGameInstance();

	Super::MapInitialize();

	for (AEMPGridSquare* gridSquare : GridSquares)
	{
		gridSquare->SetForceHidden(true);
	}
}

void AEMPCombatMapGrid::SpawnCombatSquad(UEMPSquadData* OwningSquad)
{
	AEMPCombatSquad* newSquad = GetWorld()->SpawnActorDeferred<AEMPCombatSquad>(CombatSquadClass, FTransform());
	newSquad->OwningSquadData = OwningSquad;
	newSquad->OwningGrid = this;
	newSquad->FinishSpawning(FTransform());
	newSquad->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	CombatSquads.Add(newSquad);
}

TArray<FIntPoint> AEMPCombatMapGrid::GetFriendlySquadSpawnPoints() const
{
	TArray<FIntPoint> points;
	for (auto pair : SpawnPoints)
	{
		if (pair.Value)
		{
			points.Add(pair.Key);
		}
	}
	return points;
}

TArray<FIntPoint> AEMPCombatMapGrid::GetEnemySquadSpawnPoints() const
{
	TArray<FIntPoint> points;
	for (auto pair : SpawnPoints)
	{
		if (!pair.Value)
		{
			points.Add(pair.Key);
		}
	}
	return points;
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

void AEMPCombatMapGrid::LoadFromDataAsset(class UCombatMapDataAsset* InData)
{
	GridDimensions = InData->GridSize;
	SpawnPoints = InData->SpawnPoints;
	CombatObjectives = InData->CombatObjectives;

	SpawnGrid();
}
