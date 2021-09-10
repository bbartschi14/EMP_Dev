// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../EMPUserWidget.h"
#include "EMPUWSquadInspector.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPUWSquadInspector : public UEMPUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
		void SetSquadData(class UEMPSquadData* squad);

protected:
	/** Reference to the squad that this inspector is displaying */
	UPROPERTY(Transient)
		class UEMPSquadData* SquadRepresented;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UEMPUWText* SquadNameText;


};
