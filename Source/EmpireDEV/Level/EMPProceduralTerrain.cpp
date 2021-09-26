// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPProceduralTerrain.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"

AEMPProceduralTerrain::AEMPProceduralTerrain()
{
	PrimaryActorTick.bCanEverTick = false;

	TerrainMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Terrain");
	SetRootComponent(Cast<USceneComponent>(TerrainMesh));
	TerrainMesh->bUseAsyncCooking = false;
}

void AEMPProceduralTerrain::BeginPlay()
{
	Super::BeginPlay();
}

void AEMPProceduralTerrain::CreateLandscapeMesh(TArray<int32> Heights, int32 NumberOfElevations, int32 GridWidthInAreas, float GridAreaSize, float BorderOffset)
{
	int32 textureWidth = PopulateHeightTexture(Heights, NumberOfElevations, GridWidthInAreas);
	CreateRenderTargets(textureWidth);

	TArray<int32> Triangles;
	TArray<FVector> Vertices;
	TArray<FVector2D> UVs;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;

	CreateGridMeshWelded(GridWidthInAreas * PixelsPerGridArea + 1, GridWidthInAreas * PixelsPerGridArea + 1, Triangles, Vertices, UVs, (GridAreaSize + 2 * BorderOffset ) / PixelsPerGridArea);
	ModifyVertexHeights_HeightMap(Vertices, GridWidthInAreas, textureWidth);

	CalculateNormalsForMesh(Vertices, Triangles, Normals);

	TerrainMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), Tangents, true);
	ApplyLandscapeTexture();
}

void AEMPProceduralTerrain::CreateGridMeshWelded(int32 NumX, int32 NumY, TArray<int32>& Triangles, TArray<FVector>& Vertices, TArray<FVector2D>& UVs, float GridSpacing)
{
	Triangles.Empty();
	Vertices.Empty();
	UVs.Empty();

	if (NumX >= 2 && NumY >= 2)
	{
		//FVector2D Extent = FVector2D((NumX - 1) * GridSpacing, (NumY - 1) * GridSpacing) / 2;
		FVector2D Extent = FVector2D(0);
		for (int i = 0; i < NumY; i++)
		{
			for (int j = 0; j < NumX; j++)
			{
				Vertices.Add(FVector((float)j * GridSpacing - Extent.X, (float)i * GridSpacing - Extent.Y, 0));
				UVs.Add(FVector2D((float)j / ((float)NumX - 1), (float)i / ((float)NumY - 1)));
			}
		}

		for (int i = 0; i < NumY - 1; i++)
		{
			for (int j = 0; j < NumX - 1; j++)
			{
				int idx = j + (i * NumX);
				Triangles.Add(idx);
				Triangles.Add(idx + NumX);
				Triangles.Add(idx + 1);

				Triangles.Add(idx + 1);
				Triangles.Add(idx + NumX);
				Triangles.Add(idx + NumX + 1);
			}
		}
	}
}

void AEMPProceduralTerrain::CalculateNormalsForMesh(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& Normals)
{
	if (Vertices.Num() == 0)
	{
		return;
	}

	// Number of triangles
	const int32 NumTris = Triangles.Num() / 3;
	// Number of verts
	const int32 NumVerts = Vertices.Num();

	// Map of vertex to triangles in Triangles array
	TMultiMap<int32, int32> VertToTriMap;
	// Map of vertex to triangles to consider for normal calculation
	TMultiMap<int32, int32> VertToTriSmoothMap;

	// Normal/tangents for each face
	TArray<FVector> FaceTangentZ;
	FaceTangentZ.AddUninitialized(NumTris);

	// Iterate over triangles
	for (int TriIdx = 0; TriIdx < NumTris; TriIdx++)
	{
		int32 CornerIndex[3];
		FVector P[3];

		for (int32 CornerIdx = 0; CornerIdx < 3; CornerIdx++)
		{
			// Find vert index (clamped within range)
			int32 VertIndex = FMath::Min(Triangles[(TriIdx * 3) + CornerIdx], NumVerts - 1);

			CornerIndex[CornerIdx] = VertIndex;
			P[CornerIdx] = Vertices[VertIndex];

			// Remember which triangles map to this vert
			VertToTriMap.AddUnique(VertIndex, TriIdx);
			VertToTriSmoothMap.AddUnique(VertIndex, TriIdx);
		}

		// Calculate triangle edge vectors and normal
		const FVector Edge21 = P[1] - P[2];
		const FVector Edge20 = P[0] - P[2];
		const FVector TriNormal = (Edge21 ^ Edge20).GetSafeNormal();

		FaceTangentZ[TriIdx] = TriNormal;
	}


	// Arrays to accumulate tangents into
	TArray<FVector> VertexTangentZSum;

	VertexTangentZSum.AddZeroed(NumVerts);

	// For each vertex..
	for (int VertxIdx = 0; VertxIdx < Vertices.Num(); VertxIdx++)
	{
		// Find relevant triangles for normal
		TArray<int32> SmoothTris;
		VertToTriSmoothMap.MultiFind(VertxIdx, SmoothTris);

		for (int i = 0; i < SmoothTris.Num(); i++)
		{
			int32 TriIdx = SmoothTris[i];
			VertexTangentZSum[VertxIdx] += FaceTangentZ[TriIdx];
		}
	}

	// Finally, normalize tangents and build output arrays

	Normals.Reset();
	Normals.AddUninitialized(NumVerts);

	for (int VertxIdx = 0; VertxIdx < NumVerts; VertxIdx++)
	{
		FVector& TangentZ = VertexTangentZSum[VertxIdx];
		TangentZ.Normalize();

		Normals[VertxIdx] = TangentZ;
	}
}

uint8 AEMPProceduralTerrain::GetPixelValueForGridAreaHeight(TArray<int32> Heights, int32 NumberOfElevations, int32 GridAreaIndex)
{
	if (Heights.IsValidIndex(GridAreaIndex))
	{
		uint8 value = Heights[GridAreaIndex] * 255 / (NumberOfElevations - 1);
		return value;
	}
	else
	{
		return 0;
	}
}

int32 AEMPProceduralTerrain::PopulateHeightTexture(TArray<int32> Heights, int32 NumberOfElevations, int32 GridWidthInAreas)
{
	int32 TextureWidth = GridWidthInAreas * PixelsPerGridArea + 1;
	TextureWidth = TextureWidth - 1;
	TextureWidth = FMath::Pow(2, FMath::CeilToInt(FMath::Log2(TextureWidth))); // Round up to nearest power of 2

	HeightMapTexture = NewObject<UTexture2D>();
	HeightMapTexture->PlatformData = new FTexturePlatformData();	// Then we initialize the PlatformData
	HeightMapTexture->PlatformData->SizeX = TextureWidth;
	HeightMapTexture->PlatformData->SizeY = TextureWidth;
	HeightMapTexture->PlatformData->SetNumSlices(1);
	HeightMapTexture->PlatformData->PixelFormat = EPixelFormat::PF_G8;

	uint8* Pixels = new uint8[TextureWidth * TextureWidth];

	for (int32 x = 0; x < TextureWidth; x++)
	{
		for (int32 y = 0; y < TextureWidth; y++)
		{
			int32 curentPixelIndex = ((y * TextureWidth) + x);
			Pixels[curentPixelIndex] = 0;
		}
	}

	for (int32 gridX = 0; gridX < GridWidthInAreas; gridX++)
	{
		for (int32 gridY = 0; gridY < GridWidthInAreas; gridY++)
		{
			int32 currentGridAreaIndex = gridY * GridWidthInAreas + gridX;
			int32 xOffset = gridX * PixelsPerGridArea;
			int32 yOffset = gridY * PixelsPerGridArea;

			// Populate flat grid area
			for (int32 x = 0; x < PixelsPerGridArea; x++)
			{
				for (int32 y = 0; y < PixelsPerGridArea; y++)
				{
					int32 curentPixelIndex = (((y + yOffset) * TextureWidth) + (x + xOffset));
					uint8 value = GetPixelValueForGridAreaHeight(Heights, NumberOfElevations, currentGridAreaIndex);
					Pixels[curentPixelIndex] = value;
				}
			}
		}
	}

	// Allocate first mipmap.
	FTexture2DMipMap* Mip = new(HeightMapTexture->PlatformData->Mips) FTexture2DMipMap();
	Mip->SizeX = TextureWidth;
	Mip->SizeY = TextureWidth;

	// Lock the texture so it can be modified
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* TextureData = (uint8*)Mip->BulkData.Realloc(TextureWidth * TextureWidth);
	FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * TextureWidth * TextureWidth);
	Mip->BulkData.Unlock();

	HeightMapTexture->UpdateResource();

	delete[] Pixels;

	return TextureWidth;
}

void AEMPProceduralTerrain::ModifyVertexHeights_HeightMap(TArray<FVector>& InVertices, int32 GridWidthInAreas, int32 TextureWidth)
{
	// https://github.com/mortmaire/Unreal-Engine-Access-Render-Target/blob/master/MyActor.cpp Make a texture from render target
	// https://isaratech.com/ue4-reading-the-pixels-from-a-utexture2d/ Read a texture. 
	// Somewhat garbage for runtimme. I ended up looking at the engine code in RenderTarget2D to find the readpixels operation.

	FTextureRenderTargetResource* resource = HeightMapRenderTarget->GameThread_GetRenderTargetResource();
	TArray<FColor> SurfData;
	resource->ReadPixels(SurfData);

	for (int i = 0; i < InVertices.Num(); i++)
	{
		// Position in grid dimensions
		int x = i % (GridWidthInAreas * PixelsPerGridArea + 1);
		int y = i / (GridWidthInAreas * PixelsPerGridArea + 1);

		FColor PixelColor = SurfData[TextureWidth * y + x];

		InVertices[i].Z = SingleLevelElevation * PixelColor.R;

		float noise = FMath::PerlinNoise3D(InVertices[i]);
		//UE_LOG(LogTemp, Warning, TEXT("Noise: %f, Location: %s"), noise, *InVertices[i].ToString());
		//InVertices[i].X += XNoise * noise;
		//InVertices[i].Y += YNoise * noise;
	}
}

void AEMPProceduralTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

