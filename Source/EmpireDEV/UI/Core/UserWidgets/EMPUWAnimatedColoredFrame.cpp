// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWAnimatedColoredFrame.h"
#include "EMPUWColoredFrame.h"

void UEMPUWAnimatedColoredFrame::NativeActivateBorder()
{
	AnimatedFrame->ActivateBorder();
	bIsFrameActive = true;
	ActivateBorder();
}

void UEMPUWAnimatedColoredFrame::NativeDeactivateBorder()
{
	AnimatedFrame->DeactivateBorder();
	bIsFrameActive = false;
	DeactivateBorder();
}