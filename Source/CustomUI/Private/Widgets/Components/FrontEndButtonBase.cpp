// Amigo Luis all rights reserved


#include "Widgets/Components/FrontEndButtonBase.h"

#include "CheckAndLogAndReturn.h"
#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "LogMacros.h"
#include "PrintInLog.h"
#include "UILogger.h"
#include "Subsystems/UIGameInstanceSubsystem.h"

void UFrontEndButtonBase::SetButtonText(FText NewButtonText)
{
	if (ButtonTextBlock == nullptr)
	{
		PrintInLog(SYMBOL_NAME_TEXT(ButtonTextBlock) TEXT(" is nullptr ") IN_FUNC_AND_LINE, VeryVerbose);
		return;
	}
	if (NewButtonText.IsEmpty())
	{
		PrintInLog(TEXT("Can't set empty text to button."), Warning);
		return;
	}
	ButtonTextBlock->SetText(bUseUpperCaseForButtonText ? NewButtonText.ToUpper() : NewButtonText);
}

FText UFrontEndButtonBase::GetButtonText() const
{
	if (ButtonTextBlock) return ButtonTextBlock->GetText();
	return FText();
}

void UFrontEndButtonBase::SetButtonDisplayImageBrush(const FSlateBrush& InBrush)
{
	if (ButtonLazyImage) ButtonLazyImage->SetBrush(InBrush);
}

void UFrontEndButtonBase::BroadcastUpdatedButtonDescription(const bool bNotBroadCastEmpty)
{
	if (!bNotBroadCastEmpty)
	{
		UUIGameInstanceSubsystem::Get(this)->
			OnButtonDescriptionUpdatedDelegate.Broadcast(this, FText::GetEmpty());
	}
	else
	{
		if (!ButtonTooltipText.IsEmpty()) // 这里是空就不广播了，会不会导致文本未更新的问题？
		{
			UUIGameInstanceSubsystem::Get(this)->
				OnButtonDescriptionUpdatedDelegate.Broadcast(this, ButtonTooltipText);
		}
	}
}

void UFrontEndButtonBase::ForceHoverState(bool bIsHovered)
{
	if (ButtonTextBlock == nullptr) return;
	if (bIsHovered)
	{
		// 确保样式更新
		if (const auto StylePtr = GetStyle())
		{
			ButtonTextBlock->SetStyle(StylePtr->NormalHoveredTextStyle);
		}
	}
	else
	{
		if (const auto StylePtr = GetStyle())
		{
			ButtonTextBlock->SetStyle(StylePtr->NormalTextStyle);
		}
	}
}

void UFrontEndButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetButtonText(ButtonDisplayText);
}

void UFrontEndButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();
	
	if (ButtonTextBlock == nullptr)
	{
		PrintInLog(SYMBOL_NAME_TEXT(ButtonTextBlock) 
			TEXT(" is nullptr ") IN_FUNC_AND_LINE, VeryVerbose);
		return;
	}	

	CHECK_NULL_RETURN_WARN(GetCurrentTextStyleClass());
	ButtonTextBlock->SetStyle(GetCurrentTextStyleClass());
}

void UFrontEndButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();
	BroadcastUpdatedButtonDescription(true);
}

void UFrontEndButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	BroadcastUpdatedButtonDescription(false);
}


void UFrontEndButtonBase::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	BroadcastUpdatedButtonDescription(true);
	ForceHoverState(true);
	Super::NativeOnAddedToFocusPath(InFocusEvent);
}

void UFrontEndButtonBase::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	ForceHoverState(false);
	BroadcastUpdatedButtonDescription(false);
}
