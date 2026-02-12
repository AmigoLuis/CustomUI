// Amigo Luis all rights reserved


#include "Settings/FrontendGameUserSettings.h"

#include "CheckAndLogAndReturn.h"
#include "LogMacros.h"
#include "UILogger.h"
#include "FunctionLibraries/LanguageManager.h"

UFrontendGameUserSettings::UFrontendGameUserSettings() : OverallVolume(1.0f), MusicVolume(1.0f), 
                                                         SoundFXVolume(1.0f), VoiceVolume(1.0f), AmbientVolume(1.0f), AllowBackgroundMusic(true)
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

void UFrontendGameUserSettings::SetDisplayLanguageTag(const FString& InDisplayLanguage)
{
	this->DisplayLanguageTag = InDisplayLanguage;
	FLanguageManager::Get().ChangeLanguage(InDisplayLanguage);
}

void UFrontendGameUserSettings::SetOverallVolume(const float& InVolume)
{
	OverallVolume = InVolume;
}

void UFrontendGameUserSettings::SetMusicVolume(const float& InVolume)
{
	MusicVolume = InVolume;
}

void UFrontendGameUserSettings::SetSoundFXVolume(const float& InVolume)
{
	SoundFXVolume = InVolume;
}

float UFrontendGameUserSettings::GetBrightness() const
{
	if (GEngine)
	{
		return GEngine->GetDisplayGamma();
	}
	return 0.0f;
}

void UFrontendGameUserSettings::SetBrightness(const float InBrightness)
{
	if (GEngine)
	{
		GEngine->DisplayGamma = InBrightness;
	}
}

