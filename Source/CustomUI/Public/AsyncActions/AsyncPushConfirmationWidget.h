// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncPushConfirmationWidget.generated.h"

enum class EConfirmScreenType : uint8;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmWidgetButtonClickedDelegate, 
	EConfirmScreenButtonType, ConfirmScreenButtonType);
/**
 * 
 */
UCLASS()
class CUSTOMUI_API UAsyncPushConfirmationWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (
		WorldContext = "WorldContextObject", 
		HidePin = "WorldContextObject",
		BlueprintInternalUseOnly = "true"
		), DisplayName="Push Confirmation Widget To Stack Async")
	static UAsyncPushConfirmationWidget* PushConfirmWidgetToStackAsync(
		const UObject* WorldContextObject,
		const EConfirmScreenType ConfirmScreenType,
		const FString& InTitleTextKeyInSt,
		const FString& InMessageTextKeyInSt);
	
	UFUNCTION(BlueprintCallable, meta = (
		WorldContext = "WorldContextObject", 
		HidePin = "WorldContextObject",
		BlueprintInternalUseOnly = "true"
		), DisplayName="Push Confirmation Widget To Stack Async With Full Parameter")
	static UAsyncPushConfirmationWidget* PushConfirmWidgetToStackAsyncWithFullParameter(
		const UObject* WorldContextObject,
		const EConfirmScreenType ConfirmScreenType,
		const FString& InTitleTextKeyInSt,
		const FString& InMessageTextKeyInSt,
		const FName& StringTableID);
	UPROPERTY(BlueprintAssignable)
	FOnConfirmWidgetButtonClickedDelegate OnConfirmWidgetButtonClicked;
		
private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	EConfirmScreenType CachedConfirmScreenType;
	FText CachedTitleText;
	FText CachedMessageText;

public:
	virtual void Activate() override;
};
