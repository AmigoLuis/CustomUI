// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIFunctionLibrary.generated.h"

class UWidgetActivatableBase;
/**
 * 
 */
UCLASS()
class CUSTOMUI_API UUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "UI Library")
	static TSoftClassPtr<UWidgetActivatableBase> GetWidgetSoftFromSettings(
	UPARAM(meta = (Categories = "FrontEnd.Widget")) FGameplayTag WidgetTag);
	
	UFUNCTION(BlueprintPure, Category = "UI Library")
	static TSoftObjectPtr<UTexture2D> GetSoftImageByTagFromSettings(
	UPARAM(meta = (Categories = "FrontEnd.Image")) FGameplayTag InImageTag);
	
	UFUNCTION(BlueprintPure, Category = "UI Library")
	static bool IsPlayerHaveRecord();
	UFUNCTION(BlueprintPure, Category = "UI Library")
	static bool IsPlayerHaveCompletedGameOnce();
};
