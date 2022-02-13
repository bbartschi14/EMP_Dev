// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPGridSquare.h"

// Sets default values
AEMPGridSquare::AEMPGridSquare()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Bind mouse events
	OnClicked.AddDynamic(this, &AEMPGridSquare::NativeHandleGridSquareClicked);
	OnBeginCursorOver.AddDynamic(this, &AEMPGridSquare::NativeHandleGridSquareBeginCursorOver);
	OnEndCursorOver.AddDynamic(this, &AEMPGridSquare::NativeHandleGridSquareEndCursorOver);

	bIsEnabled = true;
}

// Called when the game starts or when spawned
void AEMPGridSquare::BeginPlay()
{
	Super::BeginPlay();
}

void AEMPGridSquare::InitializeGridSquare(FIntPoint inGridCoordinate)
{
	GridCoordinate = inGridCoordinate;

	//SetLocationFromGridSize(inGridSize); // Currently, location is set when spawning in the game mode
}

FIntPoint AEMPGridSquare::GetGridCoordinate() const
{
	return GridCoordinate;
}

// Called every frame
void AEMPGridSquare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region MouseEventHandlers
void AEMPGridSquare::NativeHandleGridSquareClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("Grid square clicked, Coordinate: (%i, %i)"), GridCoordinate.X, GridCoordinate.Y);
	//HandleGridSquareClicked();
	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		OnGridSquareClicked.Broadcast(this);
	}
	else if (ButtonPressed == EKeys::RightMouseButton)
	{
		OnGridSquareRightClicked.Broadcast(this);
	}
}

void AEMPGridSquare::NativeHandleGridSquareBeginCursorOver(AActor* TouchedActor)
{
	if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("Grid square hovered, Coordinate: (%i, %i)"), GridCoordinate.X, GridCoordinate.Y);
	//HandleGridSquareBeginCursorOver();
	OnGridSquareBeginCursorOver.Broadcast(this);
}

void AEMPGridSquare::NativeHandleGridSquareEndCursorOver(AActor* TouchedActor)
{
	if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("Grid square unhovered, Coordinate: (%i, %i)"), GridCoordinate.X, GridCoordinate.Y);
	//HandleGridSquareEndCursorOver();
	OnGridSquareEndCursorOver.Broadcast(this);
}
#pragma endregion MouseEventHandlers

void AEMPGridSquare::SetLocationFromGridSize(FVector2D inGridSize)
{
	float baseHeight = GetActorLocation().Z;
	SetActorLocation(FVector(GridCoordinate.X * inGridSize.X, 
							 GridCoordinate.Y * inGridSize.Y, 
							 baseHeight));
}