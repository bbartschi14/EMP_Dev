// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EMPOrigamiLandscape.generated.h"


UCLASS()
class EMPIREDEV_API AEMPOrigamiLandscape : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEMPOrigamiLandscape();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
		void SpawnInstance(FVector location, float size);

	UFUNCTION(BlueprintImplementableEvent)
		void AnimatePaperOpening(float startValue, float endValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SpawnLandscape(int32 InGridDimensions, TArray<int32> InHeights, float InGridAreaSize, float InBorderOffset);
};
