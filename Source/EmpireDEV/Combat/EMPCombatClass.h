// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EMPCombatClass.generated.h"

UCLASS(Abstract, Blueprintable)
class EMPIREDEV_API AEMPCombatClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEMPCombatClass();

	/** Activates attack animations */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "EMP|Combat Class")
		void HandleAttackTriggered(FVector InTargetLocation);

	/** */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "EMP|Combat Class")
		void HandleSetBaseMaterial(bool bIsFriendly);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
