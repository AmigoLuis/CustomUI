// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
// #include "CommonActivatableWidgetContainerBase.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widget_PrimaryLayout.generated.h" 
class UCommonActivatableWidgetContainerBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UWidget_PrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()
	UPROPERTY(Transient)
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisteredWidgetStackMap;
protected:
	UFUNCTION(BlueprintCallable)
	void RegisterWidget(UPARAM(meta = (Categories = "FrontEnd.WidgetStack")) FGameplayTag Tag, 
		UCommonActivatableWidgetContainerBase* WidgetContainerBase);
public:
	UFUNCTION(BlueprintCallable)
	UCommonActivatableWidgetContainerBase* TryGetWidgetStack(
		UPARAM(meta = (Categories = "FrontEnd.WidgetStack")) FGameplayTag Tag);
};
