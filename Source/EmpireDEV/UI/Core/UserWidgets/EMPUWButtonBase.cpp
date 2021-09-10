// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPUWButtonBase.h"
#include "Components/Button.h"

void UEMPUWButtonBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (NativeButton)
	{
		// Remove first to avoid duplicate
		NativeButton->OnClicked.RemoveDynamic(this, &UEMPUWButtonBase::HandleOnNativeButtonClick);
		NativeButton->OnClicked.AddUniqueDynamic(this, &UEMPUWButtonBase::HandleOnNativeButtonClick);

		NativeButton->OnHovered.RemoveDynamic(this, &UEMPUWButtonBase::HandleOnNativeButtonHovered);
		NativeButton->OnHovered.AddUniqueDynamic(this, &UEMPUWButtonBase::HandleOnNativeButtonHovered);

		NativeButton->OnUnhovered.RemoveDynamic(this, &UEMPUWButtonBase::HandleOnNativeButtonUnhovered);
		NativeButton->OnUnhovered.AddUniqueDynamic(this, &UEMPUWButtonBase::HandleOnNativeButtonUnhovered);
	}
}

void UEMPUWButtonBase::HandleOnNativeButtonClick()
{
	OnClick.Broadcast();
}

void UEMPUWButtonBase::HandleOnNativeButtonHovered()
{
	OnHover.Broadcast();
}

void UEMPUWButtonBase::HandleOnNativeButtonUnhovered()
{
	OnUnhover.Broadcast();
}