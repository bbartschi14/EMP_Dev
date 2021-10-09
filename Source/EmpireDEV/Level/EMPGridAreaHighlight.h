// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EMPGridAreaHighlight.generated.h"

UCLASS()
class EMPIREDEV_API AEMPGridAreaHighlight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEMPGridAreaHighlight();

	UFUNCTION(BlueprintCallable)
		void SetForceVisible(bool inIsForceVisible);

	UFUNCTION(BlueprintCallable)
		void SetHovered(bool inIsHovered);

	UFUNCTION(BlueprintCallable)
		void SetSelected(bool inIsSelected);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void RefreshVisuals();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Transient)
		bool bIsHovered;

	UPROPERTY(BlueprintReadOnly, Transient)
		bool bIsSelected;

	UPROPERTY(BlueprintReadOnly, Transient)
		bool bIsForceVisible;
};