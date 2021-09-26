// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#pragma region OverrideFunctions
AEMPPawn::AEMPPawn()
{
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 900.f;
	CameraBoom->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AEMPPawn::BeginPlay() 
{
	Super::BeginPlay();
	// Set starting arm length based on the blueprint set variables
	if (SpringArmZoomLengths.IsValidIndex(ZoomTargetIndex))
	{
		CameraBoom->TargetArmLength = SpringArmZoomLengths[ZoomTargetIndex];
	}
}

void AEMPPawn::SetupPlayerInputComponent(UInputComponent* inputComponent)
{
	Super::SetupPlayerInputComponent(inputComponent);
	inputComponent->BindAxis("Zoom", this, &AEMPPawn::HandleZoomInput);
}

void AEMPPawn::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	LerpSpringArmLengthToZoomTarget(deltaTime);
}
#pragma endregion OverrideFunctions

#pragma region ZoomHandling
int AEMPPawn::GetZoomTargetIndex() const
{
	return ZoomTargetIndex;
}

void AEMPPawn::LerpSpringArmLengthToZoomTarget(float deltaTime)
{
	float currentLength = CameraBoom->TargetArmLength;
	float newLength = FMath::Lerp(currentLength, (float)SpringArmZoomLengths[ZoomTargetIndex], .1f * ZoomLerpSpeed);
	CameraBoom->TargetArmLength = newLength;

	if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("New Length: %f"), newLength);
}

void AEMPPawn::HandleZoomInput(float zoomAxisValue)
{
    // Update ZoomTargetIndex

	if (zoomAxisValue < -0.95f)
	{
		// Try to increase the zoom target
		if (ZoomTargetIndex + 1 < SpringArmZoomLengths.Num())
		{
			ZoomTargetIndex++;
			OnZoomTargetIndexChanged.Broadcast(ZoomTargetIndex);
		}
	}
	else if (zoomAxisValue > 0.95f)
	{
		// Try to decrease the zoom target
		if (ZoomTargetIndex - 1 >= 0)
		{
			ZoomTargetIndex--;
			OnZoomTargetIndexChanged.Broadcast(ZoomTargetIndex);
		}
	}

	if (bEnableDebugMode) UE_LOG(LogTemp, Warning, TEXT("Zoom Target Index: %i"), ZoomTargetIndex);
}
#pragma endregion ZoomHandling

