// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIGameInstanceSubsystem.generated.h"

struct FGameplayTag;
class UWidgetPrimaryLayout;
class UWidgetActivatableBase;
enum class EAsyncPushWidgetState : uint8
{
	CreatedAndBeforePush,
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
	UWidgetPrimaryLayout* CreatedPrimaryLayoutWidget;
public:
	static  UUIGameInstanceSubsystem* Get(const UObject* WorldContextObject);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UWidgetPrimaryLayout* Widget_PrimaryLayoutToRegister);
	
	void PushWidgetSoftPtrToStackAsync(TSoftClassPtr<UWidgetActivatableBase> WidgetSoftPtr, const FGameplayTag& Tag, 
		TFunction<void(EAsyncPushWidgetState, UWidgetActivatableBase*)> AsyncPushCallback) const;
};
