// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "EMPPawn.generated.h"

/**
 * Main pawn used for camera movement
 */
UCLASS()
class EMPIREDEV_API AEMPPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:

	AEMPPawn();

protected:

	virtual void Tick(float deltaTime) override;
	virtual void BeginPlay() override;

	// Setup the player input.
	virtual void SetupPlayerInputComponent(UInputComponent* inputComponent) override;

	/** Defines the discrete spring arm zoom lengths available when zooming */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int> SpringArmZoomLengths;

	/** Current index of the spring arm lengths array to interpolate towards */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ZoomTargetIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ZoomLerpSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bEnableDebugMode = false;
private:

	/** Zooming is stepped at intervals based on the spring arm 
	 *  zoom lengths array and the current target index.
	 */
	UFUNCTION()
		void HandleZoomInput(float deltaTime);
	
	/** On each tick, move a percentage towards the current zoom target, giving a smooth ease-out interpolation */
	UFUNCTION()
		void LerpSpringArmLengthToZoomTarget(float deltaTime);

	/** Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

};
