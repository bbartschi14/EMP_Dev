// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EMPCombatPlayer.generated.h"

UCLASS()
class EMPIREDEV_API AEMPCombatPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEMPCombatPlayer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		class AEMPSquad* CreateSquad();

	UFUNCTION()
		TArray<class AEMPSquad*> GetSquads();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Transient)
		TArray<class AEMPSquad*> Squads;
};
