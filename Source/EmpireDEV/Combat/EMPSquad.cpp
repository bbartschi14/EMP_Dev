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

AEMPCombatUnit* AEMPSquad::CreateCombatUnit(TSubclassOf<AEMPCombatUnit> CombatUnitClass)
{
	AEMPCombatUnit* newCombatUnit = GetWorld()->SpawnActor<AEMPCombatUnit>(CombatUnitClass);
	CombatUnits.Add(newCombatUnit);

	return newCombatUnit;
}

void AEMPSquad::SetStartingAreaCoordinate(FIntPoint areaCoordinate)
{
	CurrentAreaCoordinate = areaCoordinate;
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




