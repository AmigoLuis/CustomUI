// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIGameInstanceSubsystem.generated.h"

struct FGameplayTag;
class UWidget_PrimaryLayout;
class UWidgetActivatableBase;
enum class EAsyncPushWidgetState : uint8
{
	CreatedButNotPushed,
	CreatedAndPushed
};

/**
 * 
 */
UCLASS()
class CUSTOMUI_API UUIGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	UPROPERTY(Transient)
	UWidget_PrimaryLayout* CreatedPrimaryLayoutWidget;
public:
	static  UUIGameInstanceSubsystem* Get(const UObject* WorldContextObject);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* Widget_PrimaryLayoutToRegister);
	
	void PushWidgetSoftPtrToStackAsync(TSoftClassPtr<UWidgetActivatableBase> WidgetSoftPtr, const FGameplayTag& Tag, 
		TFunctionRef<void(EAsyncPushWidgetState, UWidgetActivatableBase&)> AsyncPushCallback);
};
