// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EMPCombatSquad.generated.h"

UCLASS()
class EMPIREDEV_API AEMPCombatSquad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEMPCombatSquad();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly)
		class UEMPSquadData* OwningSquadData;

	UPROPERTY(BlueprintReadOnly)
		class AEMPCombatMapGrid* OwningGrid;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	
};
