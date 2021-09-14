// Empire 2021. Ben Bartschi and Jacob Bartschi

#include "EMPStylesheetLibrary.h"

int32 UEMPStylesheetLibrary::GetFontSize(const EEMPFontSize& fontSize)
{
	switch (fontSize)
	{
	case EEMPFontSize::FS_LARGE:
		return 32;

	case EEMPFontSize::FS_MEDIUM:
		return 20;

	case EEMPFontSize::FS_SMALL:
		return 12;

	default:
		return 12;
	}
}

int32 UEMPStylesheetLibrary::GetPaddingSize(const EEMPPaddingSize& paddingSize)
{
	switch (paddingSize)
	{
	case EEMPPaddingSize::PS_EXTRALARGE:
		return 36;

	case EEMPPaddingSize::PS_LARGE:
		return 18;

	case EEMPPaddingSize::PS_MEDIUM:
		return 12;

	case EEMPPaddingSize::PS_SMALL:
		return 6;

	case EEMPPaddingSize::PS_EXTRASMALL:
		return 4;

	case EEMPPaddingSize::PS_NONE:
		return 0;

	default:
		return 0;
	}
}

FLinearColor UEMPStylesheetLibrary::GetColor(const EEMPColor& color)
{
	switch (color)
	{
	case EEMPColor::COL_PRIMARY:
		return FLinearColor(.9f , 0.325f, 0.0f, 1.0f);
	case EEMPColor::COL_TRANSPARENT:
		return FLinearColor(0.f, 0.f, 0.f, 0.f);
	default:
		return FLinearColor();
	}
}
