// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushWidgetSoft.generated.h"

struct FGameplayTag;
class UWidgetActivatableBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, UWidgetActivatableBase*, PushedWidget);
/**
 * 
 */
UCLASS()
class CUSTOMUI_API UAsyncAction_PushWidgetSoft : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (
		WorldContext = "WorldContextObject", 
		HidePin = "WorldContextObject",
		BlueprintInternalUseOnly = "true"
		), DisplayName="Push Soft Widget To Stack  Async")
	static UAsyncAction_PushWidgetSoft* PushWidgetSoftToStackAsync(
		const UObject* WorldContextObject,
		APlayerController* PlayerController,
		TSoftClassPtr<UWidgetActivatableBase> InWidgetSoftPtr,
		UPARAM(meta = (Categories = "FrontEnd.WidgetStack")) FGameplayTag InWidgetTag, 
		bool bFocusOnNewlyPushedWidget = true);
	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate OnWidgetCreatedAndBeforePush;
	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate OnWidgetCreatedAndPushed;
	virtual void Activate() override;

private:
	
	TWeakObjectPtr<UObject> CachedOwningWorld;
	TWeakObjectPtr<APlayerController> CachedOwningPlayerController;
	TSoftClassPtr<UWidgetActivatableBase> CachedWidgetSoftPtr;
	FGameplayTag CachedWidgetTag; 
	bool bCachedFocusOnNewlyPushedWidget = false;
};
