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
	//TODO: 目前没有真的设置音量，后续需要设置到游戏设置中，而不只是设置GetCurrentGame的值
	UFUNCTION()
	FORCEINLINE void SetCurrentGameOverallVolume(const float& InVolume);
	//*** Audio Collection Tab
private:
	// Gameplay Tab
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	// Gameplay Tab
	
	// Audio Tab
	UPROPERTY(Config)
	float CurrentGameOverallVolume;
	// Audio Tab
};
