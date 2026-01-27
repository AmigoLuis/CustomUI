// Amigo Luis all rights reserved


#include "Settings/FrontendGameUserSettings.h"

#include "UILogger.h"

UFrontendGameUserSettings::UFrontendGameUserSettings() : CurrentGameOverallVolume(1.0f)
{
}

UFrontendGameUserSettings* UFrontendGameUserSettings::Get()
{
	CHECK_NULL_RETURN_VALUE(GEngine, nullptr);
	UFrontendGameUserSettings* const Settings = Cast<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());
	if (Settings == nullptr)
	{
		PrintInLog(SYMBOL_NAME_TEXT(Cast<UFrontendGameUserSettings>(GEngine->GetGameUserSettings())) 
			TEXT(" is nullptr, please set ") 
			SYMBOL_NAME_TEXT(UFrontendGameUserSettings) 
			TEXT(" as GameUserSetting in project settings."));
	}
	return Settings;
}

void UFrontendGameUserSettings::SetCurrentGameOverallVolume(const float& InVolume)
{
	CurrentGameOverallVolume = InVolume;
}

