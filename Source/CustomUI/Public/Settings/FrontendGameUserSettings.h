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
	[[nodiscard]] FORCEINLINE FString GetCurrentGameDifficulty() const
	{
		return CurrentGameDifficulty;
	}
	UFUNCTION()
	FORCEINLINE void SetCurrentGameDifficulty(const FString& InCurrentGameDifficulty)
	{
		this->CurrentGameDifficulty = InCurrentGameDifficulty;
	}
	//*** Gameplay Collection Tab
	//*** Audio Collection Tab
	UFUNCTION()
	[[nodiscard]] FORCEINLINE float GetCurrentGameOverallVolume() const
	{
		return CurrentGameOverallVolume;
	}
	//TODO: 目前没有真的设置音量，后续需要设置到游戏设置中，而不只是设置CurrentGameOverallVolume的值
	UFUNCTION()
	FORCEINLINE void SetCurrentGameOverallVolume(const float& InVolume);
	UFUNCTION()
	[[nodiscard]] FORCEINLINE float GetCurrentGameMusicVolume() const
	{
		return CurrentGameMusicVolume;
	}
	//TODO: 目前没有真的设置音量，后续需要设置到游戏设置中，而不只是设置CurrentGameMusicVolume的值
	UFUNCTION()
	FORCEINLINE void SetCurrentGameMusicVolume(const float& InVolume);
	UFUNCTION()
	[[nodiscard]] FORCEINLINE float GetCurrentGameSoundFXVolume() const
	{
		return CurrentGameSoundFXVolume;
	}
	//TODO: 目前没有真的设置音量，后续需要设置到游戏设置中，而不只是设置CurrentGameSoundFXVolume的值
	UFUNCTION()
	FORCEINLINE void SetCurrentGameSoundFXVolume(const float& InVolume);
	
	UFUNCTION()
	[[nodiscard]] FORCEINLINE bool GetCurrentGameAllowBackgroundMusic() const
	{return CurrentGameAllowBackgroundMusic;}
	UFUNCTION()
	FORCEINLINE void SetCurrentGameAllowBackgroundMusic(const bool InAllowBackgroundMusic)
	{CurrentGameAllowBackgroundMusic = InAllowBackgroundMusic;}
	//*** Audio Collection Tab
private:
	// Gameplay Tab
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	// Gameplay Tab
	
	// Audio Tab
	UPROPERTY(Config)
	float CurrentGameOverallVolume;
	UPROPERTY(Config)
	float CurrentGameMusicVolume;
	UPROPERTY(Config)
	float CurrentGameSoundFXVolume;
	UPROPERTY(Config)
	bool CurrentGameAllowBackgroundMusic;
	// Audio Tab
};
