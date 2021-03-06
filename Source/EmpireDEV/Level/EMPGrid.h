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
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGridNotify, AEMPGrid*, Grid);

	UPROPERTY(BlueprintAssignable)
	FOnGridNotify OnGridSpawned;

	UFUNCTION(BlueprintCallable)
		int32 GetGridDimension() const;

	UFUNCTION(BlueprintCallable)
		void SetGridDimension(int32 InDimension);

	UFUNCTION(BlueprintCallable)
		TArray<int32> GetHeights() const;

	UFUNCTION(BlueprintCallable)
		void SetHeights(TArray<int32> InHeights);

	UFUNCTION(BlueprintCallable)
		int32 GetHeightOfAreaCoordinate(FIntPoint areaCoordinate) const;

	UFUNCTION(BlueprintCallable)
		void GetAllAreaCoordinates(TArray<FIntPoint>& OutAreaCoordinates) const;

	UFUNCTION(BlueprintCallable)
		class AEMPGridAreaHighlight* GetGridAreaHighlightAtCoordinate(FIntPoint areaCoordinate) const;

	UFUNCTION(BlueprintImplementableEvent)
		void RefreshGridVisuals();

	UFUNCTION(BlueprintCallable)
		void Get2DBounds(FVector2D& Min, FVector2D& Max) const;

	virtual void MapInitialize();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/** Create landscape, grid and visuals */
	UFUNCTION(BlueprintCallable)
		void SpawnGrid();

	UFUNCTION()
		void SpawnLandscape();

	UFUNCTION(BlueprintCallable)
		FVector GetWorldLocationFromGridLocation(FIntPoint gridLocation) const;

	UFUNCTION(BlueprintCallable)
		FVector GetWorldLocationFromGridArea(FIntPoint gridArea) const;

	UFUNCTION(BlueprintCallable)
		class AEMPCombatUnit* GetCombatUnitFromData(class UEMPCombatUnitData* combatUnitData) const;

	UFUNCTION()
		void HandleGridSquareClicked(class AEMPGridSquare* inGridSquare);

	UFUNCTION()
		void HandleGridSquareRightClicked(class AEMPGridSquare* inGridSquare);

	UFUNCTION()
		void HandleSetGridHighlighted(class AEMPGridSquare* inGridSquare, bool isHighlighted);

	UFUNCTION(BlueprintCallable)
		void HandleSetGridAreaHighlighted(FIntPoint areaCoordinate, bool isHighlighted);

	UFUNCTION(BlueprintCallable)
		void HandleSetGridAreaSelected(FIntPoint areaCoordinate, bool isSelected);

	UFUNCTION()
		void HandleGridSquareHovered(class AEMPGridSquare* inGridSquare);

	UFUNCTION()
		void HandleGridSquareUnhovered(class AEMPGridSquare* inGridSquare);

	UFUNCTION(BlueprintImplementableEvent)
		void OnGridSquareClicked(class AEMPGridSquare* inGridSquare);

	UFUNCTION(BlueprintImplementableEvent)
		void OnGridSquareRightClicked(class AEMPGridSquare* inGridSquare);

	UFUNCTION(BlueprintImplementableEvent)
		void OnGridSquareHovered(class AEMPGridSquare* inGridSquare);

	UFUNCTION(BlueprintImplementableEvent)
		void OnGridSquareUnhovered(class AEMPGridSquare* inGridSquare);

	UFUNCTION()
		bool IsGridCoordinateInAreaCoordinate(FIntPoint gridCoordinate, FIntPoint areaCoordinate);

	/** Helper function for getting the grid square in 1D array from 2D coordinate */
	UFUNCTION(BlueprintCallable)
		AEMPGridSquare* GetGridSquareAtCoordinate(FIntPoint gridCoordinate) const;

	UFUNCTION(BlueprintCallable)
		class AEMPCombatUnit* GetCombatUnitAtCoordinate(FIntPoint gridCoordinate) const;

	/** An area coordinate is the coordinate of the 5x5 section. I.e., the first 5 grid squares in the first 5 columns is in area (0, 0) */
	UFUNCTION(BlueprintCallable)
		FIntPoint GetAreaCoordinateOfGridCoordinate(FIntPoint gridCoordinate) const;

	UFUNCTION(BlueprintCallable)
		TArray<class AEMPGridSquare*> GetGridSquaresInArea(FIntPoint areaCoordinate) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class AEMPCombatUnit> CombatUnitClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray<class AEMPGridSquare*> GridSquares;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray<class AEMPGridAreaHighlight*> GridAreaHighlights;

	UPROPERTY(Transient, BlueprintReadWrite)
		TArray<class AEMPCombatUnit*> CombatUnits;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AEMPGridSquare> GridSquareClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AEMPGridAreaHighlight> GridAreaHighlightClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AEMPLandscape> LandscapeClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AActor> ActorRootClass;

	/** In units of grid areas, so 1x1 will make 5x5 squares */
	UPROPERTY(EditAnywhere)
		int32 GridDimensions;

	UPROPERTY(EditAnywhere)
		TArray<int32> Heights;

	UPROPERTY(EditAnywhere)
		int32 NumberOfHeightLevels;

	UPROPERTY(EditDefaultsOnly)
		float SingleGridSquareSize;

	UPROPERTY(EditDefaultsOnly)
		float BorderOffset;

	UPROPERTY(EditDefaultsOnly)
		float GridBaseHeight; 
		
	UPROPERTY(EditDefaultsOnly)
		bool bShouldSpawnAreaHighlights;

	UPROPERTY()
		int32 GridAreaSize = 5;

	UPROPERTY(Transient)
		class AEMPGridSquare* HoveredGridSquare;
		
	UPROPERTY(Transient)
		class AEMPLandscape* Landscape;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class AActor* GridSquareRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class AActor* GridAreaHighlightRoot;
};
