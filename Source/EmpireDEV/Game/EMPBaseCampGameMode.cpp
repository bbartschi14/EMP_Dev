// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPBaseCampGameMode.h"

void AEMPBaseCampGameMode::BeginPlay()
{
	Super::BeginPlay();

	SpawnGrid();
}