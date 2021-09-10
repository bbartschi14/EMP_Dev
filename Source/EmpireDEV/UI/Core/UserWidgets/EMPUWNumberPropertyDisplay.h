// Empire 2021. Ben Bartschi and Jacob Bartschi

#pragma once

#include "CoreMinimal.h"
#include "../../EMPUserWidget.h"
#include "EMPUWNumberPropertyDisplay.generated.h"

/**
 * 
 */
UCLASS()
class EMPIREDEV_API UEMPUWNumberPropertyDisplay : public UEMPUserWidget
{
	GENERATED_BODY()

public:
	/** Set the value displayed */
	UFUNCTION(BlueprintCallable)
		void SetNumber(float inNumber);

	/** Set the property label text */
	UFUNCTION(BlueprintCallable)
		void SetText(FText inText);

	/** Set the number of decimal places */
	UFUNCTION(BlueprintCallable)
		void SetDecimalPlaces(int32 inDecimalPlaces);

protected:
	virtual void SynchronizeProperties() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UEMPUWText* TextLabel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UEMPUWText* NumberLabel;

	/** Display label text */
	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		FText Text;

	/** Display number text */
	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		float Number;

	/** Decimal places of number */
	UPROPERTY(EditAnywhere, Category = "EMP UI Style")
		int32 DecimalPlaces;
};
