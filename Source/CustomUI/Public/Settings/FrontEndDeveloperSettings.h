// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "FrontEndDeveloperSettings.generated.h"

class UWidgetActivatableBase;
/**
 * 
 */
UCLASS(Config= Game, DefaultConfig, meta = (DisplayName = "FrontEnd UI Settings"))
class CUSTOMUI_API UFrontEndDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(Config, EditAnywhere, Category = "Widgets Reference", 
		meta=(ForceInlineRow, Categories = "FrontEnd.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UWidgetActivatableBase>> WidgetClassesMap;
	UPROPERTY(Config, EditAnywhere, Category = "Settings Detail Image Reference", 
		meta=(ForceInlineRow, Categories = "FrontEnd.Image"))
	TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> SettingsDetailSoftImageMap;
};
