// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"

struct FLanguageInfo
{
	FString LanguageTag;
	FString LanguageDisplayName;
};

/**
 * 
 */
class CUSTOMUI_API FLanguageManager
{
public:
	static FLanguageManager& Get();
	const TArray<FLanguageInfo>& GetAvailableLanguageInfo(const bool bForceRefresh = false);
	void ChangeLanguage(FString LanguageTagToSet);
	void TrySetDefaultLanguage();
private:
	TArray<FLanguageInfo> AllAvailableLanguageInfo;
	FString CurrentLanguageName;
	FString PlayerOSLanguageTag;

	bool IsLanguageTagSupported(const FString& LanguageTag);
	FLanguageManager(){};
	FLanguageManager(const FLanguageManager& Other) = delete;
	FLanguageManager(FLanguageManager&& Other) noexcept = delete;
	FLanguageManager& operator=(const FLanguageManager& Other) = delete;
	FLanguageManager& operator=(FLanguageManager&& Other) noexcept = delete;
};
