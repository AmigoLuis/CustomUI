// Amigo Luis all rights reserved


#include "FunctionLibraries/UIFunctionLibrary.h"

#include "CheckAndLogAndReturn.h"
#include "UILogger.h"
#include "Settings/FrontEndDeveloperSettings.h"

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
	return false;
}
