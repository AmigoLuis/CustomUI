// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CUSTOMUI_API FLanguageManager
{
public:
	// TODO:1根据用户的操作系统语言，设置默认语言
	// TODO:2列举出所有可用的语言
	// TODO:3运行时设置当前使用的语言
	static FLanguageManager& Get();
	TArray<FString> GetAvailableLanguageNames(const bool bForceRefresh = false);
	void ChangeLanguage(FString TargetCultureName);
	void TrySetDefaultLanguage();
private:
	TArray<FString> AllAvailableLanguageDisplayNames;
	TArray<FString> AllAvailableLanguageNames;
	FString CurrentLanguageName;
	FString PlayerOSLanguageName;

	FLanguageManager(){};
	FLanguageManager(const FLanguageManager& Other) = delete;
	FLanguageManager(FLanguageManager&& Other) noexcept = delete;
	FLanguageManager& operator=(const FLanguageManager& Other) = delete;
	FLanguageManager& operator=(FLanguageManager&& Other) noexcept = delete;
};
