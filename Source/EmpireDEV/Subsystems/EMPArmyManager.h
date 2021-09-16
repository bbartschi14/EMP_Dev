// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EMPArmyManager.generated.h"

/**
 * Handles persistent army data. Money/resources earned from combat and war,
 * army upgrades, and upkeep.
 */
UCLASS()
class EMPIREDEV_API UEMPArmyManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArmyResourcesChanged);

	/** Called when a value in the army resources is changed */
	UPROPERTY(BlueprintAssignable, Category = "EMP Events")
		FOnArmyResourcesChanged OnArmyResourcesChanged;

	UFUNCTION(BlueprintCallable)
		class UEMPArmyResources* GetResources() const;

protected:
	UPROPERTY(Transient)
		class UEMPArmyResources* Resources;

};
