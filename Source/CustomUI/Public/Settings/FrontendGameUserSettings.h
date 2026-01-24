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
	static UFrontendGameUserSettings* Get();
private:
	UPROPERTY(Config)
	FString CurrentGameDifficulty;

public:
	//*** Game Collection Tab
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
	//*** Game Collection Tab
};
