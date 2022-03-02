// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPLandscape.h"

AEMPLandscape::AEMPLandscape()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AEMPLandscape::BeginPlay()
{
	Super::BeginPlay();
}

void AEMPLandscape::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEMPLandscape::SpawnLandscape(int32 InGridDimensions, TArray<int32> InHeights, float InGridAreaSize, float InBorderOffset)
{
	CleanupLandscape();

	float origamiQuadDownscale = 2.0f;

	SetActorLocation(FVector(-InBorderOffset/2, -InBorderOffset/2, 0.0f));

	float landscapeAreaSize = InGridAreaSize + 2 * InBorderOffset;

	for (int i = 0; i < InGridDimensions; i++)
	{
		for (int j = 0; j < InGridDimensions; j++)
		{
			SpawnInstance(FVector(i * landscapeAreaSize, j * landscapeAreaSize, 0.0f), landscapeAreaSize / origamiQuadDownscale);
		}
	}
}

