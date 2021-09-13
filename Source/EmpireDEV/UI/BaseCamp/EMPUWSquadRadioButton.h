// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../Core/UserWidgets/Buttons/EMPUWRadioButton.h"
#include "../../Combat/Structs/FEMPSquadData.h"
#include "EMPUWSquadRadioButton.generated.h"

/**
 * Used as a single radio button representing a squad in the squad editor
 */
UCLASS()
class EMPIREDEV_API UEMPUWSquadRadioButton : public UEMPUWRadioButton
{
	GENERATED_BODY()

public:
	/** Assign the squad that this button should represent */
	UFUNCTION(BlueprintCallable)
		void SetSquadData(UEMPSquadData* inSquad);

	/** Get the squad that this button represents */
	UFUNCTION(BlueprintCallable)
		UEMPSquadData* GetSquadData() const;

protected:
	/** Reference to the squad that this button represents */
	UPROPERTY(Transient)
		UEMPSquadData* SquadRepresented;
};
