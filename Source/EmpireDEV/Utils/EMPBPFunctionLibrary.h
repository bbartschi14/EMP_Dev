// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/LatentActionManager.h"
#include "EMPBPFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	 * Customized delay for passing in the world context object manually.
	 *
	 * @param WorldContext	World context.
	 * @param Duration 		length of delay (in seconds).
	 * @param LatentInfo 	The latent action.
	 */
		UFUNCTION(BlueprintCallable, Category = "Utilities|FlowControl", meta = (Latent, LatentInfo = "LatentInfo", Duration = "0.2", Keywords = "sleep"))
		static void	DelayWorld(const UObject* WorldContextObject, float Duration, struct FLatentActionInfo LatentInfo);

};
