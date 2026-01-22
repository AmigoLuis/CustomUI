// Amigo Luis all rights reserved


#include "Widgets/FrontEndButtonBase.h"
#include "CommonTextBlock.h"
#include "UILogger.h"
#include "Subsystems/UIGameInstanceSubsystem.h"

void UFrontEndButtonBase::SetButtonText(FText NewButtonText)
{
	CHECK_NULL_RETURN(ButtonTextBlock);
	if (NewButtonText.IsEmpty())
	{
		PrintInLog(TEXT("Can't set empty text to button."), Warning);
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
	CHECK_NULL_RETURN_WARN(GetCurrentTextStyleClass());
	ButtonTextBlock->SetStyle(GetCurrentTextStyleClass());
}

void UFrontEndButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();
	if (!ButtonTooltipText.IsEmpty()) // 这里是空就不广播了，会不会导致文本未更新的问题？
	{
		UUIGameInstanceSubsystem::Get(this)->
			OnButtonDescriptionUpdatedDelegate.Broadcast(this, ButtonTooltipText);
	}
}

void UFrontEndButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	UUIGameInstanceSubsystem::Get(this)->
		OnButtonDescriptionUpdatedDelegate.Broadcast(this, FText::GetEmpty());
}
