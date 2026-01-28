// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMUI_API UFrontendGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:
	UFrontendGameUserSettings();
	static UFrontendGameUserSettings* Get();
	//*** Gameplay Collection Tab
	UFUNCTION()
	[[nodiscard]] FORCEINLINE FString GetDifficulty() const
	{
		return Difficulty;
	}
	UFUNCTION()
	FORCEINLINE void SetDifficulty(const FString& InDifficulty)
	{
		this->Difficulty = InDifficulty;
	}
	//*** Gameplay Collection Tab
	//*** Audio Collection Tab
	UFUNCTION()
	[[nodiscard]] FORCEINLINE float GetOverallVolume() const
	{
		return OverallVolume;
	}
	//TODO: 目前没有真的设置音量，后续需要设置到游戏设置中，而不只是设置OverallVolume的值
	UFUNCTION()
	FORCEINLINE void SetOverallVolume(const float& InVolume);
	UFUNCTION()
	[[nodiscard]] FORCEINLINE float GetMusicVolume() const
	{
		return MusicVolume;
	}
	//TODO: 目前没有真的设置音量，后续需要设置到游戏设置中，而不只是设置MusicVolume的值
	UFUNCTION()
	FORCEINLINE void SetMusicVolume(const float& InVolume);
	UFUNCTION()
	[[nodiscard]] FORCEINLINE float GetSoundFXVolume() const
	{
		return SoundFXVolume;
	}
	//TODO: 目前没有真的设置音量，后续需要设置到游戏设置中，而不只是设置SoundFXVolume的值
	UFUNCTION()
	FORCEINLINE void SetSoundFXVolume(const float& InVolume);
	
	UFUNCTION()
	[[nodiscard]] FORCEINLINE bool GetAllowBackgroundMusic() const
	{return AllowBackgroundMusic;}
	UFUNCTION()
	FORCEINLINE void SetAllowBackgroundMusic(const bool InAllowBackgroundMusic)
	{AllowBackgroundMusic = InAllowBackgroundMusic;}
	
	UFUNCTION()
	[[nodiscard]] FORCEINLINE bool GetUseHDRAudio() const
	{return UseHDRAudio;}
	UFUNCTION()
	FORCEINLINE void SetUseHDRAudio(const bool InUseHDRAudio)
	{UseHDRAudio = InUseHDRAudio;}
	//*** Audio Collection Tab
private:
	// Gameplay Tab
	UPROPERTY(Config)
	FString Difficulty;
	// Gameplay Tab
	
	// Audio Tab
	UPROPERTY(Config)
	float OverallVolume;
	UPROPERTY(Config)
	float MusicVolume;
	UPROPERTY(Config)
	float SoundFXVolume;
	UPROPERTY(Config)
	bool AllowBackgroundMusic;
	UPROPERTY(Config)
	bool UseHDRAudio;
	// Audio Tab
};
