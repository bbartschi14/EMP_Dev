// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/EEMPCombatDirection.h"
#include "EMPCombatUnit.generated.h"

USTRUCT(BlueprintType)
struct FEMPCombatUnitMeshData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
		bool bIsFriendlyUnit;
};

USTRUCT(BlueprintType)
struct FEMPCombatUnitMoveData
{
	GENERATED_USTRUCT_BODY()

public:
	FEMPCombatUnitMoveData() : bIsDrop(false) {}

	UPROPERTY(BlueprintReadWrite)
		bool bIsDrop;
};

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
	UFUNCTION(BlueprintCallable)
		void InitializeCombatUnitData(class UEMPCombatUnitData* combatUnitData);

	UFUNCTION(BlueprintImplementableEvent)
		void HandleCombatUnitDataInitialized();

	/** */
	UFUNCTION(BlueprintCallable)
		void InitializeToGridSquare(FIntPoint inCoordinate, float animationTime);

	UFUNCTION(BlueprintCallable)
		FIntPoint GetGridCoordinate() const;

	UFUNCTION(BlueprintCallable)
		class UEMPCombatUnitData* GetCombatUnitData() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void TurnToDirection(EEMPCombatDirection direction);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetupCombatUnitMesh(FEMPCombatUnitMeshData meshData);

	/** */
	UFUNCTION(BlueprintCallable)
		void MoveToGridSquare(FIntPoint inCoordinate, bool bNotifyWhenFinished);

	/** Moves to grid square with specific time length */
	UFUNCTION(BlueprintCallable)
		void MoveToGridSquare_Timed(FIntPoint inCoordinate, float animationTime);

	/** Play animation or other indicator of moving to a location */
	UFUNCTION(BlueprintImplementableEvent, Category = "EMP|Combat Unit")
		void HandleMoveToLocation(bool bNotifyWhenFinished);

	UFUNCTION(BlueprintImplementableEvent, Category = "EMP|Combat Unit")
		void HandleMoveToLocation_Timed(float animationTime, const FEMPCombatUnitMoveData& InData);

	/** Play animation or other indicator of a failed movement */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "EMP|Combat Unit")
		void HandleMovementFailed(bool bNotifyWhenFinished);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "EMP|Combat Unit")
		void HandleMovementFailed_Timed(float animationTime);

	UFUNCTION(BlueprintCallable)
		void TriggerAttack(FVector InTargetLocation);

	/** Activates attack animations */
	UFUNCTION(BlueprintImplementableEvent, Category = "EMP|Combat Unit")
		void HandleAttackTriggered();

	UFUNCTION(BlueprintCallable)
		void SetReadyStance(bool isReady);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FIntPoint GridCoordinate = FIntPoint(-1, -1);

	UPROPERTY(Transient)
		int32 CachedDamage;

	UPROPERTY(Transient, VisibleAnywhere)
		class UEMPCombatUnitData* CombatUnitData;

	UPROPERTY(BlueprintReadWrite, Transient)
		FVector TargetLocation;

	UPROPERTY(BlueprintReadWrite, Transient)
		bool bIsReadyStance;

	/** */
	UFUNCTION()
		void SetGridCoordinate(FIntPoint inCoordinate);

	/** */
	UFUNCTION()
		void HandleCombatUnitDeath();
};
