// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "EMPArmyResources.generated.h"

/**
 * Class version of FEMPArmyResources
 */
UCLASS(BlueprintType)
class EMPIREDEV_API UEMPArmyResources : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient, BlueprintReadWrite)
		int32 Money;
};

/**
 * Holds all the serializable/persistent data for an army's resources
 */
USTRUCT(BlueprintType)
struct FEMPArmyResourcesStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UEMPArmyResources* GetArmyResources() const
	{
		UEMPArmyResources* armyResources = NewObject<UEMPArmyResources>();
		armyResources->Money = Money;
		return armyResources;
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Money;
};
