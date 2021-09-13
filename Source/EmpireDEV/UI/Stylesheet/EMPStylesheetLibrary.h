// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetStyleEnumsEMP.h"
#include "EMPStylesheetLibrary.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPStylesheetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Stylesheet")
		static int32 GetFontSize(const EEMPFontSize& fontSize);

	UFUNCTION(BlueprintCallable, Category = "Stylesheet")
		static int32 GetPaddingSize(const EEMPPaddingSize& paddingSize);

	UFUNCTION(BlueprintCallable, Category = "Stylesheet")
		static FLinearColor GetColor(const EEMPColor& color);
};
