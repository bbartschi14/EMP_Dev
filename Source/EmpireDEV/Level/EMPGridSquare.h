// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EMPGridSquare.generated.h"

/*
 * Represents a single grid square on the level. Should store personal coordinates
 * and broadcast its coordinate on click.
 */
UCLASS()
class EMPIREDEV_API AEMPGridSquare : public AActor
{
	GENERATED_BODY()
	
public:	
#pragma region GridDelegates
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGridSquareClicked, AEMPGridSquare*, GridSquare);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGridSquareBeginCursorOver, AEMPGridSquare*, GridSquare);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGridSquareEndCursorOver, AEMPGridSquare*, GridSquare);

	/** Called when the grid square is clicked on */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnGridSquareClicked OnGridSquareClicked;

	/** Called when the grid square is moused over */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnGridSquareBeginCursorOver OnGridSquareBeginCursorOver;

	/** Called when the mouse over ends */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnGridSquareEndCursorOver OnGridSquareEndCursorOver;
#pragma endregion GridDelegates

	// Sets default values for this actor's properties
	AEMPGridSquare();

	/** Call after spawning to setup grid coordinate and other info */
	UFUNCTION()
		void InitializeGridSquare(FIntPoint inGridCoordinate);

	UFUNCTION()
		FIntPoint GetGridCoordinate() const;

	/** Blueprint response to setting highlighted */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "EMP|Grid")
		void SetHighlighted(bool isHighlighted);

	/** Blueprint response to setting selected */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "EMP|Grid")
		void SetSelected(bool isSelected);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region MouseEventHandlers
	/** Handle click event on the grid square */
	UFUNCTION()
		void NativeHandleGridSquareClicked(AActor* TouchedActor, FKey ButtonPressed);

	/** Blueprint response to click event on the grid square */
	UFUNCTION(BlueprintImplementableEvent, Category = "EMP|Grid")
		void HandleGridSquareClicked();

	/** Handle start hover event on the grid square */
	UFUNCTION()
		void NativeHandleGridSquareBeginCursorOver(AActor* TouchedActor);

	/** Blueprint response to hover event on the grid square */
	UFUNCTION(BlueprintImplementableEvent, Category = "EMP|Grid")
		void HandleGridSquareBeginCursorOver();

	/** Handle end hover event on the grid square */
	UFUNCTION()
		void NativeHandleGridSquareEndCursorOver(AActor* TouchedActor);

	/** Blueprint response to end hover event on the grid square */
	UFUNCTION(BlueprintImplementableEvent, Category = "EMP|Grid")
		void HandleGridSquareEndCursorOver();
#pragma endregion MouseEventHandlers

	/** Set the actor location based on its grid coordinate and the grid size */
	UFUNCTION()
		void SetLocationFromGridSize(FVector2D inGridSize);

	UPROPERTY(BlueprintReadOnly)
		FIntPoint GridCoordinate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bEnableDebugMode;

	UPROPERTY(Transient)
		class AEMPCombatUnit* CombatUnitOnGridSquare;

	UPROPERTY(BlueprintReadWrite)
		bool bIsHighlighted;

	UPROPERTY(BlueprintReadWrite)
		bool bIsSelected;
};
