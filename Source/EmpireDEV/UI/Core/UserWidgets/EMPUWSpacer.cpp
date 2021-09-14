// Empire 2021. Ben Bartschi and Jacob Bartschi

#include "EMPUWSpacer.h"
#include "Components/Spacer.h"

void UEMPUWSpacer::SynchronizeProperties()
{
	SetWidth(WidthSize);
	SetHeight(HeightSize);

	Super::SynchronizeProperties();
}

void UEMPUWSpacer::SetWidth(EEMPPaddingSize inWidthSize)
{
	if (NativeSpacer)
	{
		WidthSize = inWidthSize;
		NativeSpacer->SetSize(FVector2D(UEMPStylesheetLibrary::GetPaddingSize(WidthSize), UEMPStylesheetLibrary::GetPaddingSize(HeightSize)));
	}
}

void UEMPUWSpacer::SetHeight(EEMPPaddingSize inHeightSize)
{
	if (NativeSpacer)
	{
		HeightSize = inHeightSize;
		NativeSpacer->SetSize(FVector2D(UEMPStylesheetLibrary::GetPaddingSize(WidthSize), UEMPStylesheetLibrary::GetPaddingSize(HeightSize)));
	}
}
