// Amigo Luis all rights reserved


#include "Subsystems/UIGameInstanceSubsystem.h"

#include "UILogger.h"
#include "Engine/AssetManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/WidgetActivatableBase.h"
#include "Widgets/WidgetPrimaryLayout.h"


UUIGameInstanceSubsystem* UUIGameInstanceSubsystem::Get(const UObject* WorldContextObject)
{
	CHECK_NULL_RETURN_VALUE(GEngine, nullptr);
	CHECK_NULL_RETURN_VALUE(WorldContextObject, nullptr);
	
	const UWorld* World = 
		GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	CHECK_NULL_RETURN_VALUE(World, nullptr);
	
	UUIGameInstanceSubsystem* UIGameInstanceSubsystem = UGameInstance::GetSubsystem<UUIGameInstanceSubsystem>(World->GetGameInstance());
	CHECK_NULL_RETURN_VALUE(UIGameInstanceSubsystem, nullptr);
	return UIGameInstanceSubsystem;
}
bool UUIGameInstanceSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UGameInstance* GameInstance = Cast<UGameInstance>(Outer);
	CHECK_NULL_RETURN_VALUE(GameInstance, false);
	if (!GameInstance->IsDedicatedServerInstance())
	{
		TArray<UClass*> DerivedClasses;
		GetDerivedClasses(GetClass(), DerivedClasses);
		return DerivedClasses.IsEmpty();
	}
	return false;
}

void UUIGameInstanceSubsystem::RegisterCreatedPrimaryLayoutWidget(UWidgetPrimaryLayout* Widget_PrimaryLayoutToRegister)
{
	CHECK_NULL_RETURN(Widget_PrimaryLayoutToRegister);
	CreatedPrimaryLayoutWidget = Widget_PrimaryLayoutToRegister;
	PrintInLog("Registered Primary Layout.");
}

void UUIGameInstanceSubsystem::PushWidgetSoftPtrToStackAsync(TSoftClassPtr<UWidgetActivatableBase> WidgetSoftPtr,
	const FGameplayTag& Tag, 
	TFunctionRef<void(EAsyncPushWidgetState, UWidgetActivatableBase&)> AsyncPushCallback)
{
	if (WidgetSoftPtr.IsNull()) {LOG_NULL_PTR(WidgetSoftPtr); return;}
	using UWidgetContainer = UCommonActivatableWidgetContainerBase;
	
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WidgetSoftPtr.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this, WidgetSoftPtr, Tag, AsyncPushCallback]()
		{
			UClass* LoadedClass = WidgetSoftPtr.Get();
			CHECK_NULL_RETURN(LoadedClass);
			CHECK_NULL_RETURN(this->CreatedPrimaryLayoutWidget);
			
			UWidgetContainer* FoundWidgetStack = CreatedPrimaryLayoutWidget->FindWidgetStack(Tag);
			CHECK_NULL_RETURN(FoundWidgetStack);
			
			UWidgetActivatableBase* WidgetCreated = 
				FoundWidgetStack->AddWidget<UWidgetActivatableBase>(LoadedClass, 
					[AsyncPushCallback](UWidgetActivatableBase& WidgetCreatedBeforePush)
			{
				AsyncPushCallback(EAsyncPushWidgetState::CreatedAndBeforePush, WidgetCreatedBeforePush);
			});
			CHECK_NULL_RETURN(WidgetCreated);
			AsyncPushCallback(EAsyncPushWidgetState::CreatedAndPushed, *WidgetCreated);
			PrintInLog("Successfully pushed widget to stack asynchronously.", Display);
		}));
}

