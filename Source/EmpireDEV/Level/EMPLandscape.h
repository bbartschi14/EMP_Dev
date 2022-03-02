// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EMPLandscape.generated.h"


UCLASS()
class EMPIREDEV_API AEMPLandscape : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEMPLandscape();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
		void SpawnInstance(FVector location, float size);

	UFUNCTION(BlueprintImplementableEvent)
		void CleanupLandscape();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SpawnLandscape(int32 InGridDimensions, TArray<int32> InHeights, float InGridAreaSize, float InBorderOffset);
};
