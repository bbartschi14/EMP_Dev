// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EMPProceduralTerrain.generated.h"

UCLASS()
class EMPIREDEV_API AEMPProceduralTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEMPProceduralTerrain();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void CreateLandscapeMesh(TArray<int32> Heights, int32 NumberOfElevations, int32 GridWidthInAreas, float GridAreaSize, float BorderOffset);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UProceduralMeshComponent* TerrainMesh;

	void CreateGridMeshWelded(int32 NumX, int32 NumY, TArray<int32>& Triangles, TArray<FVector>& Vertices, TArray<FVector2D>& UVs, float GridSpacing);
	void CalculateNormalsForMesh(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& Normals);
	uint8 GetPixelValueForGridAreaHeight(TArray<int32> Heights, int32 NumberOfElevations, int32 GridAreaIndex);

	int32 PopulateHeightTexture(TArray<int32> Heights, int32 NumberOfElevations, int32 GridWidthInAreas);

	void ModifyVertexHeights_HeightMap(TArray<FVector>& InVertices, int32 GridWidthInAreas, int32 TextureWidth);

	UFUNCTION(BlueprintImplementableEvent)
		void ApplyLandscapeTexture();

	UFUNCTION(BlueprintImplementableEvent)
		void CreateRenderTargets(int32 TextureWidth);

	UPROPERTY(BlueprintReadWrite, Transient, EditAnywhere)
		UTexture2D* HeightMapTexture;

	UPROPERTY(BlueprintReadWrite, Transient, EditAnywhere)
		UTextureRenderTarget2D* HeightMapRenderTarget;

	UPROPERTY(EditDefaultsOnly)
		int32 PixelsPerGridArea = 10;

	UPROPERTY(EditDefaultsOnly)
		float SingleLevelElevation;

};
