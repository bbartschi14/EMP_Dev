// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EMPArrowSplineMesh.generated.h"

UCLASS()
class EMPIREDEV_API AEMPArrowSplineMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEMPArrowSplineMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void InitializeArrow(FVector startLocation, FVector endLocatiion, class UEMPCombatActionSkill* owningAction);


};
