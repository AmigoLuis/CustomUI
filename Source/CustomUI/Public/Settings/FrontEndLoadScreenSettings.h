// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FrontEndLoadScreenSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game,DefaultConfig)
class CUSTOMUI_API UFrontEndLoadScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	TSubclassOf<UUserWidget> GetLoadingScreenWidgetClass() const;
	
	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings")
	TSoftClassPtr<UUserWidget> SoftLoadingScreenWidgetClass;
	
	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings")
	float HoldLoadingScreenExtraSeconds = 3.0f;
	
	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings")
	bool bShouldShowLoadingScreenInEditor = false;
};
