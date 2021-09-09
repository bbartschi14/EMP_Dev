// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EMPSquadManager.generated.h"

/**
 * Used to manage the player's squad data over the entire lifetime of the game.
 * When a level is started, the game mode will pull in squad data from here.
 */
UCLASS()
class EMPIREDEV_API UEMPSquadManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	UFUNCTION(BlueprintCallable)
		TArray<class UEMPSquadData*> GetSquadData() const;
private:

	UPROPERTY(Transient)
		TArray<class UEMPSquadData*> Squads;

	/** When loading a squad, the squad manager uses this to check that the squad data is valid.
	  * If possible, it fixes invalid data and proceeds. For example, if two units in a squad have
	  * the same desired location, it will change one of them to an available space and continue as normal.
	  */
	UFUNCTION()
		bool ValidateSquad(class UEMPSquadData* squadToValidate);

	UFUNCTION()
		bool DoesCombatUnitHaveUniqueLocationInSquad(class UEMPSquadData* squad, class UEMPCombatUnitData* originalUnit);

	UFUNCTION()
		FIntPoint FindUniqueLocationInSquad(class UEMPSquadData* squad);
};