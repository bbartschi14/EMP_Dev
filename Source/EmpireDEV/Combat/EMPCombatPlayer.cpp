// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPCombatPlayer.h"
#include "EMPSquad.h"

// Sets default values
AEMPCombatPlayer::AEMPCombatPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEMPCombatPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEMPCombatPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AEMPSquad* AEMPCombatPlayer::CreateSquad()
{
	AEMPSquad* newSquad = GetWorld()->SpawnActor<AEMPSquad>();
	Squads.Add(newSquad);

	return newSquad;
}

TArray<class AEMPSquad*> AEMPCombatPlayer::GetSquads()
{
	return Squads;
}

