// Amigo Luis all rights reserved


#include "FunctionLibraries/UIFunctionLibrary.h"

#include "CheckAndLogAndReturn.h"
#include "UILogger.h"
#include "Settings/FrontEndDeveloperSettings.h"
#include "Widgets/UserMessageBase.h"

TSoftClassPtr<UWidgetActivatableBase> UUIFunctionLibrary::GetWidgetSoftFromSettings(const FGameplayTag WidgetTag)
{
	TSoftClassPtr<UWidgetActivatableBase> WidgetSoft;
	const UFrontEndDeveloperSettings* Settings = GetDefault<UFrontEndDeveloperSettings>();
	CHECK_NULL_RETURN_VALUE(Settings, WidgetSoft);
	if (!Settings->WidgetClassesMap.Contains(WidgetTag))
	{
		PrintInLog(TEXT("Can't find widget under the tag: ") + WidgetTag.ToString(), Error);
		return WidgetSoft;
	}
	return Settings->WidgetClassesMap.FindRef(WidgetTag);
}

TSoftObjectPtr<UTexture2D> UUIFunctionLibrary::GetSoftImageByTagFromSettings(const FGameplayTag InImageTag)
{
	TSoftObjectPtr<UTexture2D> ImageSoft;
	const UFrontEndDeveloperSettings* Settings = GetDefault<UFrontEndDeveloperSettings>();
	CHECK_NULL_RETURN_VALUE(Settings, ImageSoft);
	if (!Settings->SettingsDetailSoftImageMap.Contains(InImageTag))
	{
		PrintInLog(TEXT("Can't find soft image under the tag: ") + InImageTag.ToString(), Error);
		return ImageSoft;
	}
	return Settings->SettingsDetailSoftImageMap.FindRef(InImageTag);
}

bool UUIFunctionLibrary::IsPlayerHaveRecord()
{
	return false;
}

bool UUIFunctionLibrary::IsPlayerHaveCompletedGameOnce()
{
	// TODO: 实现这个检测玩家游戏进度函数
	return false;
}

void UUIFunctionLibrary::ShowChildUserMessageWidget(APlayerController* OwningPC, const FString& InDetailTextKey,
	const TSubclassOf<UUserMessageBase> UserMessageWidgetClass, const FString& InTitleTextKey,
	const float InTimeToLiveMs)
{
	CHECK_NULL_RETURN(UserMessageWidgetClass);
	CHECK_NULL_RETURN(OwningPC);
	static TWeakObjectPtr<UUserMessageBase> PreviousMessageWidget;
	if (PreviousMessageWidget.IsValid())
	{
		PreviousMessageWidget->DestroySelf();
	}
	if (UUserMessageBase* WidgetInstance = CreateWidget<UUserMessageBase>(OwningPC, UserMessageWidgetClass))
	{
		WidgetInstance->TimeToLiveMs = InTimeToLiveMs;
		WidgetInstance->SetUserMessageTitleText(InTitleTextKey);
		WidgetInstance->SetUserMessageDetailText(InDetailTextKey);
		const FString& MessageLog = FString::Format(TEXT("UserMsg TitleKey:{0}, DetailKey:{1}"), 
			{InTitleTextKey, InDetailTextKey});
		PrintInLogVerbose(MessageLog);
		WidgetInstance->AddToViewport();
		PreviousMessageWidget = WidgetInstance;
	}
}

void UUIFunctionLibrary::ShowUserMessageWidget(APlayerController* OwningPC, const FString& InDetailTextKey,
	const FString& InTitleTextKey, const float InTimeToLiveMs)
{
	const TCHAR* DefaultUIMessageBpClassPath = TEXT("/Game/Blueprints/Widgets/WBP_UserMessage.WBP_UserMessage_C");
	static UClass* DefaultWidgetClass = LoadClass<UUserMessageBase>(nullptr, DefaultUIMessageBpClassPath);
	ShowChildUserMessageWidget(
		OwningPC,
		InDetailTextKey,
		DefaultWidgetClass,
		InTitleTextKey,
		InTimeToLiveMs);
}