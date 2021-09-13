#pragma once

#include "CoreMinimal.h"
#include "WidgetStyleEnumsEMP.generated.h"

// Used to pick available font sizes
UENUM(BlueprintType)
enum class EEMPFontSize : uint8 {
	FS_LARGE		UMETA(DisplayName = "Large"),
	FS_MEDIUM		UMETA(DisplayName = "Medium"),
	FS_SMALL		UMETA(DisplayName = "Small"),
};

// Used to pick available padding sizes
UENUM(BlueprintType)
enum class EEMPPaddingSize : uint8 {
	PS_LARGE		UMETA(DisplayName = "Large"),
	PS_MEDIUM		UMETA(DisplayName = "Medium"),
	PS_SMALL		UMETA(DisplayName = "Small"),
	PS_EXTRASMALL		UMETA(DisplayName = "Extra Small"),
	PS_NONE			UMETA(DisplayName = "None"),
};

// Used to pick available colors
UENUM(BlueprintType)
enum class EEMPColor : uint8 {
	COL_PRIMARY		UMETA(DisplayName = "Primary Color"),
	COL_TRANSPARENT		UMETA(DisplayName = "Transparent Color"),
};