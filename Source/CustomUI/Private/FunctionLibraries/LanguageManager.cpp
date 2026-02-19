// Amigo Luis all rights reserved


#include "FunctionLibraries/LanguageManager.h"

#include "CheckAndLogAndReturn.h"
#include "PrintInLog.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Settings/FrontendGameUserSettings.h"

FLanguageManager& FLanguageManager::Get()
{
	static FLanguageManager FLanguageManager;
	return FLanguageManager;
}

const TArray<FLanguageInfo>& FLanguageManager::GetAvailableLanguageInfo(const bool bForceRefresh)
{   
	if (!bForceRefresh && AllAvailableLanguageInfo.Num() > 0)
	{
		return AllAvailableLanguageInfo;
	}
	// 1. 获取所有已本地化的语言代码 (IETF标签, 如 "zh-Hans", "ja", "fr")
	const TArray<FString>& SupportedLanguageNames =
		UKismetInternationalizationLibrary::GetLocalizedCultures(true);
	if (SupportedLanguageNames.IsEmpty())
	{
		PrintInLog(TEXT("UKismetInternationalizationLibrary::GetLocalizedCultures(true) is empty"));	
	}
	for (const FString& LanguageName : SupportedLanguageNames)
	{
		// 2. 通过语言代码获取该语言的“显示名称”（该语言自己的名字）
		FString DisplayName = 
			UKismetInternationalizationLibrary::GetCultureDisplayName(LanguageName, false);
		AllAvailableLanguageInfo.Emplace(LanguageName, DisplayName);
		PrintInLogDisplay(FString::Format(TEXT("Detected new language, IETF tag: {0}, DisplayName: {1}."),
			{LanguageName,DisplayName}));
	}

	return AllAvailableLanguageInfo;
}

void FLanguageManager::ChangeLanguage(FString LanguageTagToSet)
{
	// 方法 A：使用内置库函数（最推荐，简单且安全）
	// LanguageTagToSet 传入类似 "en", "zh-Hans", "ja"
	CurrentLanguageName = LanguageTagToSet;
	UKismetInternationalizationLibrary::SetCurrentCulture(LanguageTagToSet, true);
	PrintInLogDisplay(FString::Format(TEXT("Language changed to: {0} ."), 
		{LanguageTagToSet}));
}

void FLanguageManager::TrySetDefaultLanguage()
{
	const UFrontendGameUserSettings* FrontendGameUserSettings = UFrontendGameUserSettings::Get();
	CHECK_NULL_RETURN(FrontendGameUserSettings);
	if (const FString& SavedDisplayLanguageTag = FrontendGameUserSettings->GetDisplayLanguageTag(); 
		SavedDisplayLanguageTag.IsEmpty())
	{
		PrintInLogDisplay(TEXT("No SavedDisplayLanguageTag."));
	}
	else
	{
		const bool bSavedDisplayLanguageTagSupported = IsLanguageTagSupported(SavedDisplayLanguageTag);
		PrintInLogDisplay(FString::Format(TEXT("SavedDisplayLanguageTag is '{0}', it's {1}."),
			{SavedDisplayLanguageTag, 
				bSavedDisplayLanguageTagSupported ? TEXT("supported"):TEXT("not supported")}));
		if (bSavedDisplayLanguageTagSupported)
		{
			ChangeLanguage(SavedDisplayLanguageTag);
			return;
		}
	}
	// 获取操作系统当前文化代码（如 "en-US" 或 "zh-CN"），跨平台支持。
	PlayerOSLanguageTag = FInternationalization::Get().GetCurrentCulture()->GetName();
	PrintInLogDisplay(TEXT("Player OS language IETF tag is :") + PlayerOSLanguageTag);
	
	// 判断操作系统文化是否被支持，否则使用默认文化（如第一个支持文化或英文）
	FString LanguageTagToSet = PlayerOSLanguageTag;
	if (IsLanguageTagSupported(LanguageTagToSet))
	{
		PrintInLogDisplay(FString::Format(TEXT("Player OS language IETF tag '{0}' is supported. Using it."), 
			{PlayerOSLanguageTag}));
	}
	else
	{
		if (const FString EnglishTag = TEXT("en"); IsLanguageTagSupported(EnglishTag))
		{
			LanguageTagToSet = EnglishTag;
		} 
		else if (GetAvailableLanguageInfo().Num() > 0)
		{
			LanguageTagToSet = GetAvailableLanguageInfo()[0].LanguageTag;
		}
		else
		{
			LanguageTagToSet = EnglishTag;
		}
		PrintInLogDisplay(FString::Format(TEXT("Player OS language IETF tag '{0}' is not supported, fallback to {1}."), 
			{PlayerOSLanguageTag, LanguageTagToSet}));
	}
	ChangeLanguage(LanguageTagToSet);
}

bool FLanguageManager::IsLanguageTagSupported(const FString& LanguageTag)
{
	for (const FLanguageInfo& AvailableLanguageInfo : GetAvailableLanguageInfo())
	{
		if (AvailableLanguageInfo.LanguageTag == LanguageTag) return true;
	}
	return false;
}
