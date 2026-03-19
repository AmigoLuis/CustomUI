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
	[[nodiscard]] FString GetDifficulty() const
	{
		return Difficulty;
	}
	UFUNCTION()
	void SetDifficulty(const FString& InDifficulty)
	{
		this->Difficulty = InDifficulty;
	}
	
	UFUNCTION()
	[[nodiscard]] FString GetDisplayLanguageTag() const
	{
		return DisplayLanguageTag;
	}
	UFUNCTION()
	void SetDisplayLanguageTag(const FString& InDisplayLanguage);
	//*** Gameplay Collection Tab
	//*** Audio Collection Tab
	UFUNCTION()
	[[nodiscard]] float GetOverallVolume() const
	{
		return OverallVolume;
	}
	//TODO1: 目前没有真的设置音量，后续需要设置到游戏设置中，而不只是设置OverallVolume的值
	UFUNCTION()
	void SetOverallVolume(const float& InVolume);
	UFUNCTION()
	[[nodiscard]] float GetMusicVolume() const
	{
		return MusicVolume;
	}
	// TODO: 目前拖动滑条会比较卡，推测是因为每次修改都会存储，考虑实现阶段性保存，就是修改参数后2s没有其他操作才去异步保存，防止修改滑条浮点属性时高频同步存储数据带来的卡顿
	// TODO: 保存用户设置成功后，通过消息通知widget去通知用户保存已完成
	//TODO1: 目前没有真的设置音量，后续需要设置到游戏设置中，而不只是设置MusicVolume的值
	UFUNCTION()
	void SetMusicVolume(const float& InVolume);
	UFUNCTION()
	[[nodiscard]] float GetSoundFXVolume() const
	{
		return SoundFXVolume;
	}
	//TODO1: 目前没有真的设置音量，后续需要设置到游戏设置中，而不只是设置SoundFXVolume的值
	UFUNCTION()
	void SetSoundFXVolume(const float& InVolume);
	
	UFUNCTION()
	[[nodiscard]] float GetVoiceVolume() const
	{return VoiceVolume;}
	UFUNCTION()
	void SetVoiceVolume(const float InVoiceVolume)
	{VoiceVolume = InVoiceVolume;}
	
	UFUNCTION()
	[[nodiscard]] float GetAmbientVolume() const
	{return AmbientVolume;}
	UFUNCTION()
	void SetAmbientVolume(const float InAmbientVolume)
	{AmbientVolume = InAmbientVolume;}
	
	UFUNCTION()
	[[nodiscard]] bool GetAllowBackgroundMusic() const
	{return AllowBackgroundMusic;}
	UFUNCTION()
	void SetAllowBackgroundMusic(const bool InAllowBackgroundMusic)
	{AllowBackgroundMusic = InAllowBackgroundMusic;}
	
	//*** Audio Collection Tab
	//*** Video Collection Tab
	UFUNCTION()
	[[nodiscard]] float GetBrightness() const;
	UFUNCTION()void SetBrightness(const float InBrightness);
	//*** Video Collection Tab
private:
	// Gameplay Tab
	UPROPERTY(Config)
	FString Difficulty;
	UPROPERTY(Config)
	FString DisplayLanguageTag;
	// Gameplay Tab
	
	// Audio Tab
	UPROPERTY(Config)
	float OverallVolume;
	UPROPERTY(Config)
	float MusicVolume;
	UPROPERTY(Config)
	float SoundFXVolume;
	UPROPERTY(Config)
	float VoiceVolume;
	UPROPERTY(Config)
	float AmbientVolume;
	UPROPERTY(Config)
	bool AllowBackgroundMusic;
	// Audio Tab
};
