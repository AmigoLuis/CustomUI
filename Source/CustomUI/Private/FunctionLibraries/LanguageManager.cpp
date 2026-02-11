// Amigo Luis all rights reserved


#include "FunctionLibraries/LanguageManager.h"

#include "PrintInLog.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"
#include "Kismet/KismetInternationalizationLibrary.h"

FLanguageManager& FLanguageManager::Get()
{
	static FLanguageManager FLanguageManager;
	return FLanguageManager;
}

TArray<FString> FLanguageManager::GetAvailableLanguageNames(const bool bForceRefresh)
{   
	if (!bForceRefresh
		&& AllAvailableLanguageDisplayNames.Num() > 0 
		&& AllAvailableLanguageNames.Num() > 0)
	{
		return AllAvailableLanguageDisplayNames;
	}
	// 1. 获取所有已本地化的语言代码 (IETF标签, 如 "zh-Hans", "ja", "fr")
	AllAvailableLanguageNames = 
		UKismetInternationalizationLibrary::GetLocalizedCultures(true);

	for (const FString& CultureName : AllAvailableLanguageNames)
	{
		// 2. 通过语言代码获取该语言的“显示名称”（该语言自己的名字）
		FString DisplayName = 
			UKismetInternationalizationLibrary::GetCultureDisplayName(CultureName, true);
		AllAvailableLanguageDisplayNames.Add(DisplayName);
		PrintInLogDisplay(TEXT("Detected language: ") + DisplayName);
	}

	return AllAvailableLanguageDisplayNames;
}

void FLanguageManager::ChangeLanguage(FString TargetCultureName)
{
	// 方法 A：使用内置库函数（最推荐，简单且安全）
	// TargetCultureName 传入类似 "en", "zh-Hans", "ja"
	CurrentLanguageName = TargetCultureName;
	UKismetInternationalizationLibrary::SetCurrentCulture(TargetCultureName, true);
	PrintInLogDisplay(FString::Format(TEXT("Language changed to: {0} ."), 
		{TargetCultureName}));
}

void FLanguageManager::TrySetDefaultLanguage()
{
	// 获取操作系统当前文化代码（如 "en-US" 或 "zh-CN"），跨平台支持。
	PlayerOSLanguageName = FInternationalization::Get().GetCurrentCulture()->GetName();
	PrintInLogDisplay(TEXT("Player OS language is :") + PlayerOSLanguageName);
	// 获取项目支持的所有本地化文化代码
	GetAvailableLanguageNames();
	TArray<FString> SupportedCultures = AllAvailableLanguageNames;
	
	// 判断操作系统文化是否被支持，否则使用默认文化（如第一个支持文化或英文）
	FString TargetCulture = PlayerOSLanguageName;
	if (SupportedCultures.Contains(TargetCulture))
	{
		PrintInLogDisplay(FString::Format(TEXT("OS culture :{0} is supported. Using it."), 
			{PlayerOSLanguageName}));
	}
	else
	{
		if (const FString EnglishCulture = TEXT("en"); SupportedCultures.Contains(EnglishCulture))
		{
			TargetCulture = EnglishCulture;
		} 
		else if (SupportedCultures.Num() > 0)
		{
			TargetCulture = SupportedCultures[0];
		}
		else
		{
			TargetCulture = EnglishCulture;
		}
		PrintInLogDisplay(FString::Format(TEXT("OS culture {0} is not supported, fallback to {1}."), 
			{PlayerOSLanguageName, TargetCulture}));
	}
	ChangeLanguage(TargetCulture);
}