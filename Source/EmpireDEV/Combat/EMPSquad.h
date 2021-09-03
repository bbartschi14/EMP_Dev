// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EMPSquad.generated.h"

/*
 * Squads start in the "awaiting orders" state at the start of a turn.
 * When given a movement order, they will be in the "Move queued" state, meaning
 * that they can't be given another order or rearranged. If the squad is rearranged,
 * it will be put in the "Rearranged" state, and can be continuously rearranged but
 * not given a movement order.
 */
UENUM(BlueprintType)
enum class ESquadStateEMP : uint8
{
	SS_AWAITING_ORDERS     UMETA(DisplayName = "Awaiting Orders"),
	SS_MOVE_QUEUED     UMETA(DisplayName = "Move queued"),
	SS_REARRANGED     UMETA(DisplayName = "Rearranged"),
};

UCLASS()
class EMPIREDEV_API AEMPSquad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEMPSquad();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** */
	UFUNCTION()
		class AEMPCombatUnit* CreateCombatUnit(TSubclassOf<class AEMPCombatUnit> CombatUnitClass);

	/** */
	UFUNCTION()
		void SetStartingAreaCoordinate(FIntPoint areaCoordinate);

	/** */
	UFUNCTION()
		void MoveToAreaCoordinate(FIntPoint areaCoordinate);

	/** */
	UFUNCTION()
		void HandleMovementFailed();

	/** */
	UFUNCTION(BlueprintCallable)
		FIntPoint GetCurrentAreaCoordinate() const;

	UFUNCTION()
		class AEMPCombatUnit* GetCombatUnitAtGridCoordinate(FIntPoint gridCoordinate);

	UFUNCTION()
		void SetCurrentSquadState(ESquadStateEMP newState);

	/** */
	UFUNCTION(BlueprintCallable)
		ESquadStateEMP GetCurrentSquadState() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	UPROPERTY(Transient)
		TArray<class AEMPCombatUnit*> CombatUnits;

	UPROPERTY()
		FIntPoint CurrentAreaCoordinate;

	UPROPERTY()
		ESquadStateEMP CurrentSquadState;
};
