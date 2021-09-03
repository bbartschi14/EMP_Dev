// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPCombatUnit.h"

// Sets default values
AEMPCombatUnit::AEMPCombatUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEMPCombatUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEMPCombatUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEMPCombatUnit::InitializeToGridSquare(FIntPoint inCoordinate)
{
	SetGridCoordinate(inCoordinate);
	HandleMoveToLocation(false);
}

void AEMPCombatUnit::MoveToGridSquare(FIntPoint inCoordinate, bool bNotifyWhenFinished)
{
	SetGridCoordinate(inCoordinate);
	HandleMoveToLocation(bNotifyWhenFinished);
}

void AEMPCombatUnit::SetGridCoordinate(FIntPoint inCoordinate)
{
	GridCoordinate = inCoordinate;
}

FIntPoint AEMPCombatUnit::GetGridCoordinate() const
{
	return GridCoordinate;
}


