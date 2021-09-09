// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPSquad.h"
#include "EMPCombatUnit.h"

// Sets default values
AEMPSquad::AEMPSquad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEMPSquad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEMPSquad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FIntPoint GetPerpendicularOfIntPoint_Clockwise(FIntPoint inIntPoint)
{
	return FIntPoint(inIntPoint.Y, -inIntPoint.X);
}

FIntPoint GetPerpendicularOfIntPoint_CounterClockwise(FIntPoint inIntPoint)
{
	return FIntPoint(-inIntPoint.Y, inIntPoint.X);
}


AEMPCombatUnit* AEMPSquad::CreateCombatUnit(TSubclassOf<AEMPCombatUnit> CombatUnitClass)
{
	AEMPCombatUnit* newCombatUnit = GetWorld()->SpawnActor<AEMPCombatUnit>(CombatUnitClass);
	CombatUnits.Add(newCombatUnit);
	newCombatUnit->OnCombatUnitDeath.AddUniqueDynamic(this, &AEMPSquad::HandleCombatUnitDeath);

	return newCombatUnit;
}

TArray<AEMPCombatUnit*> AEMPSquad::GetCombatUnitsInSquad() const
{
	return CombatUnits;
}

bool AEMPSquad::AdvanceCombatUnits_SingleLine(FIntPoint direction, int lineIndex, int numberOfSpacesToMove, bool bShouldNotifyWhenFinished)
{
	bool bIsAnyUnitMoving = false;
	for (AEMPCombatUnit* unit : CombatUnits)
	{
		FIntPoint unitCoordinate = unit->GetGridCoordinate();
		FIntPoint unitCoordinateInArea = FIntPoint(unitCoordinate.X - (CurrentAreaCoordinate.X * 5), unitCoordinate.Y - (CurrentAreaCoordinate.Y * 5));

		bool bShouldMoveUnit = false;

		if (direction.X == 0)
		{
			// Moving row
			if (direction.Y == 1 && unitCoordinateInArea.X == 4 - lineIndex)
			{
				bShouldMoveUnit = true;
			}
			else if (direction.Y == -1 && unitCoordinateInArea.X == lineIndex)
			{
				bShouldMoveUnit = true;
			}
		}
		else if (direction.Y == 0)
		{
			// Moving column
			if (direction.X == 1 && unitCoordinateInArea.Y == lineIndex)
			{
				bShouldMoveUnit = true;
			}
			else if (direction.X == -1 && unitCoordinateInArea.Y == 4 - lineIndex)
			{
				bShouldMoveUnit = true;
			}
		}

		// Don't allow units to move more than 1 area coordinate away **Broken**
		//FIntPoint newCoordinate = unitCoordinateInArea + direction;
		/*if (newCoordinate.X >= 9 || newCoordinate.X <= -4 || newCoordinate.Y >= 9 || newCoordinate.Y <= -4)
		{
			bShouldMoveUnit = false;
		}*/

		if (bShouldMoveUnit)
		{
			if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("Is unit notifying? %s"), bShouldNotifyWhenFinished && !bIsAnyUnitMoving ? TEXT("yes") : TEXT("no"));

			unit->MoveToGridSquare(unitCoordinate + direction * numberOfSpacesToMove, bShouldNotifyWhenFinished && !bIsAnyUnitMoving);
			bIsAnyUnitMoving = true;

		}
	}

	return bIsAnyUnitMoving;
}

bool AEMPSquad::ResetCombatUnitsAfterCombat(FIntPoint direction, bool bShouldNotifyWhenFinished)
{

	bool bIsAnyUnitMoving = false;

	FIntPoint startingGridCoodinate = GetCurrentAreaCoordinate() * 5;
	if (direction.X == -1)
	{
		startingGridCoodinate.X += 4;
		startingGridCoodinate.Y += 4;
	} 
	else if (direction.Y == 1)
	{
		startingGridCoodinate.X += 4;
	}
	else if (direction.Y == -1)
	{
		startingGridCoodinate.Y += 4;
	}

	if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("Starting grid coordinate: %s"), *startingGridCoodinate.ToString());
	if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("Starting direction: %s"), *direction.ToString());

	for (int lineIndex = 0; lineIndex < 5; lineIndex++)
	{
		AEMPCombatUnit* mostAdvancedUnit = nullptr;
		int32 spacesToRetreat = 0;
		for (int i = 9; i > 4; i--)
		{
			FIntPoint lineIndexOffset = GetPerpendicularOfIntPoint_CounterClockwise(direction) * lineIndex;
			FIntPoint coordinateToCheck = (startingGridCoodinate + direction * i) + lineIndexOffset;
			if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("Coordinate To Check: %s"), *coordinateToCheck.ToString());

			mostAdvancedUnit = GetCombatUnitAtGridCoordinate(coordinateToCheck);
			if (mostAdvancedUnit)
			{
				if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("Found unit: %s"), *mostAdvancedUnit->GetName());
				spacesToRetreat = i - 4;
				break;
			}
		}
		if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("Moving units in line %i by %i spaces"), lineIndex, spacesToRetreat);
		bIsAnyUnitMoving = AdvanceCombatUnits_SingleLine(direction, lineIndex, -spacesToRetreat, !bIsAnyUnitMoving && bShouldNotifyWhenFinished) || bIsAnyUnitMoving;
	}

	return bIsAnyUnitMoving;
}

void AEMPSquad::SetStartingAreaCoordinate(FIntPoint areaCoordinate)
{
	CurrentAreaCoordinate = areaCoordinate;
}

void AEMPSquad::SetSquadData(UEMPSquadData* squadData)
{
	SquadData = squadData;
}

UEMPSquadData* AEMPSquad::GetSquadData() const
{
	return SquadData;
}

void AEMPSquad::MoveToAreaCoordinate(FIntPoint areaCoordinate)
{
	FIntPoint areaCoordinateDelta = areaCoordinate - CurrentAreaCoordinate;
	CurrentAreaCoordinate = areaCoordinate;

	for (int i = 0; i < CombatUnits.Num(); i++)
	{
		AEMPCombatUnit* unit = CombatUnits[i];
		FIntPoint currentGridCoordinate = unit->GetGridCoordinate();
		FIntPoint destinationGridCoordinate = FIntPoint(currentGridCoordinate.X + areaCoordinateDelta.X * 5, currentGridCoordinate.Y + areaCoordinateDelta.Y * 5);

		bool bNotifyWhenFinished = i == CombatUnits.Num() - 1; // Last moving unit will notify when finished
		unit->MoveToGridSquare(destinationGridCoordinate, bNotifyWhenFinished);
	}

}

void AEMPSquad::HandleMovementFailed()
{
	for (int i = 0; i < CombatUnits.Num(); i++)
	{
		AEMPCombatUnit* unit = CombatUnits[i];
		bool bNotifyWhenFinished = i == CombatUnits.Num() - 1; // Last moving unit will notify when finished
		unit->HandleMovementFailed(bNotifyWhenFinished);
	}
}

FIntPoint AEMPSquad::GetCurrentAreaCoordinate() const
{
	return CurrentAreaCoordinate;
}

AEMPCombatUnit* AEMPSquad::GetCombatUnitAtGridCoordinate(FIntPoint gridCoordinate)
{
	for (AEMPCombatUnit* unit : CombatUnits)
	{
		if (unit->GetGridCoordinate() == gridCoordinate)
		{
			return unit;
		}
	}

	return nullptr;
}

void AEMPSquad::SetCurrentSquadState(ESquadStateEMP newState)
{
	CurrentSquadState = newState;
}

ESquadStateEMP AEMPSquad::GetCurrentSquadState() const
{
	return CurrentSquadState;
}

void AEMPSquad::HandleCombatUnitDeath(AEMPCombatUnit* combatUnit)
{
	// Clean up combat unit that died

	CombatUnits.Remove(combatUnit);
	combatUnit->Destroy();
}



