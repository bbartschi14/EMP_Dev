// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPCombatClass.h"

// Sets default values
AEMPCombatClass::AEMPCombatClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEMPCombatClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEMPCombatClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

