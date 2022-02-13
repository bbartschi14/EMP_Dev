// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EMPHUD.generated.h"

/**
* The HUD that controls loading the main UI root for a given level
***********************************************************************************/

UCLASS(Abstract)
class EMPIREDEV_API AEMPHUD : public AHUD
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
		void ShowBaseCampUI();
	UFUNCTION(BlueprintCallable)
		void ShowCombatMapUI();

	UFUNCTION(BlueprintCallable)
		void HideUI();

protected:
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> BaseCampMenuClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> CombatMapMenuClass;

	// Keep a pointer to be able to hide it
	UPROPERTY()
		class UUserWidget* CurrentRoot;
};
