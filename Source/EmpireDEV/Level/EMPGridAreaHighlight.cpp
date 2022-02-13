// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPGridAreaHighlight.h"

AEMPGridAreaHighlight::AEMPGridAreaHighlight()
{
	PrimaryActorTick.bCanEverTick = false;
	bIsEnabled = true;
}

void AEMPGridAreaHighlight::SetForceVisible(bool inIsForceVisible)
{
	bIsForceVisible = inIsForceVisible;
	RefreshVisuals();
}

void AEMPGridAreaHighlight::SetHovered(bool inIsHovered)
{
	bIsHovered = inIsHovered;
	RefreshVisuals();
}

void AEMPGridAreaHighlight::SetSelected(bool inIsSelected)
{
	bIsSelected = inIsSelected;
	RefreshVisuals();
}

void AEMPGridAreaHighlight::SetEnabled(bool isEnabled)
{
	bIsEnabled = isEnabled;
	RefreshVisuals();
}

void AEMPGridAreaHighlight::BeginPlay()
{
	Super::BeginPlay();
}

