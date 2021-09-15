// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EMPGrid.generated.h"

UCLASS()
class EMPIREDEV_API AEMPGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEMPGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Create grid squares from member properties */
	UFUNCTION()
		void SpawnGrid();

	UFUNCTION(BlueprintCallable)
		FVector GetWorldLocationFromGridLocation(FIntPoint gridLocation);

	UFUNCTION()
		virtual void HandleGridSquareClicked(class AEMPGridSquare* inGridSquare);

	UFUNCTION()
		virtual void HandleSetGridHighlighted(class AEMPGridSquare* inGridSquare, bool isHighlighted);

	UFUNCTION()
		void HandleSetGridAreaHighlighted(FIntPoint areaCoordinate, bool isHighlighted);

	UFUNCTION()
		void HandleGridSquareHovered(class AEMPGridSquare* inGridSquare);

	UFUNCTION()
		void HandleGridSquareUnhovered(class AEMPGridSquare* inGridSquare);

	UFUNCTION()
		bool IsGridCoordinateInAreaCoordinate(FIntPoint gridCoordinate, FIntPoint areaCoordinate);

	/** Helper function for getting the grid square in 1D array from 2D coordinate */
	UFUNCTION()
		AEMPGridSquare* GetGridSquareAtCoordinate(FIntPoint gridCoordinate) const;

	/** An area coordinate is the coordinate of the 5x5 section. I.e., the first 5 grid squares in the first 5 columns is in area (0, 0) */
	UFUNCTION()
		FIntPoint GetAreaCoordinateOfGridCoordinate(FIntPoint gridCoordinate) const;

	UFUNCTION()
		TArray<class AEMPGridSquare*> GetGridSquaresInArea(FIntPoint areaCoordinate) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class AEMPCombatUnit> CombatUnitClass;

	UPROPERTY(Transient)
		TArray<class AEMPGridSquare*> GridSquares;

	UPROPERTY(Transient, BlueprintReadWrite)
		TArray<class AEMPCombatUnit*> CombatUnits;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AEMPGridSquare> GridSquareClass;

	UPROPERTY(EditDefaultsOnly)
		FIntPoint GridDimensions;

	UPROPERTY(EditDefaultsOnly)
		FVector2D SingleGridSquareSize;

	UPROPERTY(EditDefaultsOnly)
		float GridBaseHeight;

	UPROPERTY()
		int32 GridAreaSize = 5;

	UPROPERTY(Transient)
		class AEMPGridSquare* HoveredGridSquare;

};
