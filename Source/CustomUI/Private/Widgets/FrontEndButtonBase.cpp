// Amigo Luis all rights reserved


#include "Widgets/FrontEndButtonBase.h"
#include "CommonTextBlock.h"
#include "UILogger.h"

void UFrontEndButtonBase::SetButtonText(FText NewButtonText)
{
	CHECK_NULL_RETURN(ButtonTextBlock);
	if (NewButtonText.IsEmpty())
	{
		PrintInLog(TEXT("Can't set empty text to button."), Error);
		return;
	}
	ButtonTextBlock->SetText(bUseUpperCaseForButtonText ? NewButtonText.ToUpper() : NewButtonText);
}

void UFrontEndButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetButtonText(ButtonDisplayText);
}

void UFrontEndButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();
	CHECK_NULL_RETURN(ButtonTextBlock);
	ButtonTextBlock->SetStyle(GetCurrentTextStyleClass());
}
