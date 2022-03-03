// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPCombatUnit.h"

// Sets default values
AEMPCombatUnit::AEMPCombatUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEMPCombatUnit::TriggerAttack(FVector InTargetLocation)
{
	TargetLocation = InTargetLocation;
	HandleAttackTriggered();
}

void AEMPCombatUnit::SetReadyStance(bool isReady)
{
	bIsReadyStance = isReady;
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

void AEMPCombatUnit::InitializeCombatUnitData(class UEMPCombatUnitData* combatUnitData)
{
	CombatUnitData = combatUnitData;
	HandleCombatUnitDataInitialized();
}

UEMPCombatUnitData* AEMPCombatUnit::GetCombatUnitData() const
{
	return CombatUnitData;
}

void AEMPCombatUnit::InitializeToGridSquare(FIntPoint inCoordinate, float animationTime)
{
	SetGridCoordinate(inCoordinate);
	FEMPCombatUnitMoveData data;
	data.bIsDrop = true;
	HandleMoveToLocation_Timed(animationTime, data);
}

void AEMPCombatUnit::MoveToGridSquare(FIntPoint inCoordinate, bool bNotifyWhenFinished)
{
	SetGridCoordinate(inCoordinate);
	HandleMoveToLocation(bNotifyWhenFinished);
}

void AEMPCombatUnit::MoveToGridSquare_Timed(FIntPoint inCoordinate, float animationTime)
{
	SetGridCoordinate(inCoordinate);
	FEMPCombatUnitMoveData data;
	HandleMoveToLocation_Timed(animationTime, data);
}

void AEMPCombatUnit::SetGridCoordinate(FIntPoint inCoordinate)
{
	GridCoordinate = inCoordinate;
}

FIntPoint AEMPCombatUnit::GetGridCoordinate() const
{
	return GridCoordinate;
}

void AEMPCombatUnit::HandleCombatUnitDeath()
{
	OnCombatUnitDeath.Broadcast(this);
}


