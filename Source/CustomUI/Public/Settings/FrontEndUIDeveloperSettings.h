// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "FrontEndUIDeveloperSettings.generated.h"

class UWidgetActivatableBase;
/**
 * 
 */
UCLASS(Config= Game, DefaultConfig, meta = (DisplayName = "FrontEnd UI Settings"))
class CUSTOMUI_API UFrontEndUIDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(Config, EditAnywhere, Category = "Widgets Reference", 
		meta=(ForceInlineRow, Categories = "FrontEnd.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UWidgetActivatableBase>> WidgetClassesMap;
};
