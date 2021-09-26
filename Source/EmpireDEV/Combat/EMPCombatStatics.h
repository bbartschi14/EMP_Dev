// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Enums/EEMPCombatDirection.h"
#include "EMPCombatStatics.generated.h"

/**
 * Helper functions for combat logic flows
 */
UCLASS()
class EMPIREDEV_API UEMPCombatStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/** Finds a combat unit in an array of squads at the specificed combat location  */
	UFUNCTION(BlueprintCallable, Category = "Combat")
		static class UEMPCombatUnitData* FindCombatUnitAtLocation(TArray<class UEMPSquadData*> squadsToCheck, FIntPoint combatLocation);

	/** Rotates a local coordinate to a new direction. Assumes starting direction is UP  */
	UFUNCTION(BlueprintCallable, Category = "Combat")
		static FIntPoint RotateLocalGridCoordinate(FIntPoint localCoordinate, EEMPCombatDirection direction);

	/** Transforms a local coordinate in an area coordinate to global grid coordinate  */
	UFUNCTION(BlueprintCallable, Category = "Combat")
		static FIntPoint TransformGridCoordinate_LocalToGlobal(FIntPoint localCoordinate, FIntPoint areaCoordinate);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		static bool IsGridCoordinateInArea(FIntPoint gridCoordinate, FIntPoint areaCoordinate);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		static FIntPoint GetPerpendicularDirection_Clockwise(FIntPoint inDirection);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		static FIntPoint GetPerpendicularDirection_CounterClockwise(FIntPoint inDirection);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		static EEMPCombatDirection IntPointToDirection(FIntPoint inIntPoint);
};
