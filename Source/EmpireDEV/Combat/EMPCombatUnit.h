// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EMPCombatUnit.generated.h"

UCLASS()
class EMPIREDEV_API AEMPCombatUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatUnitDeath, AEMPCombatUnit*, DeadUnit);

	// Sets default values for this actor's properties
	AEMPCombatUnit();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Called when the combat unit dies */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnCombatUnitDeath OnCombatUnitDeath;

	/** */
	UFUNCTION()
	void InitializeToGridSquare(FIntPoint inCoordinate);

	UFUNCTION()
		FIntPoint GetGridCoordinate() const;

	/** */
	UFUNCTION()
		void TakeCachedDamage(int32 damageToCache);

	/** */
	UFUNCTION()
		void ResolvedCachedDamage();

	/** */
	UFUNCTION()
	void MoveToGridSquare(FIntPoint inCoordinate, bool bNotifyWhenFinished);

	/** Play animation or other indicator of moving to a location */
	UFUNCTION(BlueprintImplementableEvent, Category = "EMP|Combat Unit")
	void HandleMoveToLocation(bool bNotifyWhenFinished);

	/** Play animation or other indicator of a failed movement */
	UFUNCTION(BlueprintImplementableEvent, Category = "EMP|Combat Unit")
		void HandleMovementFailed(bool bNotifyWhenFinished);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FIntPoint GridCoordinate = FIntPoint(0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 MaxHealth = 10;

	UPROPERTY(Transient)
		int32 CurrentHealth;

	UPROPERTY(Transient)
		int32 CachedDamage;

	/** */
	UFUNCTION()
	void SetGridCoordinate(FIntPoint inCoordinate);

	/** */
	UFUNCTION()
		void HandleCombatUnitDeath();
};
