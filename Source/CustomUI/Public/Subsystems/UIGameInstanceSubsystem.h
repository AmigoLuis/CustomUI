// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIGameInstanceSubsystem.generated.h"

enum class EConfirmScreenButtonType : uint8;
enum class EConfirmScreenType : uint8;
struct FGameplayTag;
class UWidgetPrimaryLayout;
class UWidgetActivatableBase;
class UFrontEndButtonBase;
enum class EAsyncPushWidgetState : uint8
{
	CreatedAndBeforePush,
	CreatedAndPushed
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionUpdatedDelegate, 
	UFrontEndButtonBase*, ButtonToUpdate, FText, DescriptionTextToUpdate);

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
	static UUIGameInstanceSubsystem* Get(const UObject* WorldContextObject);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UWidgetPrimaryLayout* Widget_PrimaryLayoutToRegister);
	
	void PushWidgetSoftPtrToStackAsync(TSoftClassPtr<UWidgetActivatableBase> WidgetSoftPtr, 
		const FGameplayTag& WidgetStackTag, 
		TFunction<void(EAsyncPushWidgetState, UWidgetActivatableBase*)> AsyncPushCallback) const;
	void PushConfirmWidgetToModalStackAsync(EConfirmScreenType ConfirmScreenType, 
		const FText& InWidgetTitle,
		const FText& InWidgetMessage,
		const TFunction<void(EConfirmScreenButtonType)>& ConfirmCallback);
	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionUpdatedDelegate OnButtonDescriptionUpdatedDelegate;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
