// Amigo Luis all rights reserved


#include "Widgets/UserMessageBase.h"

#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "LogMacros.h"
#include "PrintInLog.h"
#include "Blueprint/WidgetTree.h"
#include "Widgets/StringTableLocations.h"

#define ST_LOCATION_USER_MESSAGE ST_UN_ASSORTED


void UUserMessageBase::DestroySelf()
{
	if (bDestroyed) return;
	bDestroyed = true;
	RemoveFromParent();
}

void UUserMessageBase::SetUserMessageTitleText(const FString& InTitleTextKey)
{
	UserMessageTitleTextBlock->SetText(
		GET_VALUE_FOR_KEY_FROM_ST_DIRECT(TEXT(ST_LOCATION_USER_MESSAGE), InTitleTextKey));
}

void UUserMessageBase::SetUserMessageDetailText(const FString& InDetailTextKey)
{
	UserMessageDetailTextBlock->SetText(
		GET_VALUE_FOR_KEY_FROM_ST_DIRECT(TEXT(ST_LOCATION_USER_MESSAGE), InDetailTextKey));
}

void UUserMessageBase::SetUserMessageLazyImage()
{
	if (UserMessageLazyImage == nullptr)
	{
		PrintInLogDisplay(SYMBOL_NAME_TEXT(UserMessageLazyImage) TEXT(" is nullptr ") IN_FUNC_AND_LINE);
		return;
	}
	// TODO: 目前没有实现设置消息的图片
	// UserMessageLazyImage->SetBrushFromLazyTexture()
}

void UUserMessageBase::NativeConstruct()
{
	Super::NativeConstruct();
	if (TimeToLiveMs > 0.f)
	{
		const float DelaySeconds = TimeToLiveMs / 1000.f;
		if (const UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(DestroyTimerHandle, this, 
				&UUserMessageBase::DestroySelf, DelaySeconds, false);
		}
	}
}
