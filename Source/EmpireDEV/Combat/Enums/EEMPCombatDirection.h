#pragma once

#include "CoreMinimal.h"
#include "EEMPCombatDirection.generated.h"

UENUM(BlueprintType)
enum class EEMPCombatDirection : uint8 {
	CD_UP		UMETA(DisplayName = "Up"),
	CD_RIGHT		UMETA(DisplayName = "Right"),
	CD_DOWN		UMETA(DisplayName = "Down"),
	CD_LEFT		UMETA(DisplayName = "Left"),
};