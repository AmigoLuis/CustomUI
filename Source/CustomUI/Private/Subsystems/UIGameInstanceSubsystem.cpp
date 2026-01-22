// Amigo Luis all rights reserved


#include "Subsystems/UIGameInstanceSubsystem.h"

#include "FrontEndGameplayTags.h"
#include "UILogger.h"
#include "Engine/AssetManager.h"
#include "FunctionLibraries/UIFunctionLibrary.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/WidgetActivatableBase.h"
#include "Widgets/WidgetConfirmation.h"
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
	PrintInLog("Registered Primary Layout.", Display);
}

void UUIGameInstanceSubsystem::PushWidgetSoftPtrToStackAsync(TSoftClassPtr<UWidgetActivatableBase> WidgetSoftPtr,
	const FGameplayTag& WidgetStackTag, 
	TFunction<void(EAsyncPushWidgetState, UWidgetActivatableBase*)> AsyncPushCallback) const
{
	if (WidgetSoftPtr.IsNull()) {LOG_NULL_PTR(WidgetSoftPtr); return;}
	using UWidgetContainer = UCommonActivatableWidgetContainerBase;
	
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WidgetSoftPtr.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this, WidgetSoftPtr, WidgetStackTag, AsyncPushCallback]()
		{
			UClass* LoadedClass = WidgetSoftPtr.Get();
			CHECK_NULL_RETURN(LoadedClass);
			CHECK_NULL_RETURN(this->CreatedPrimaryLayoutWidget);
			
			UWidgetContainer* FoundWidgetStack = CreatedPrimaryLayoutWidget->FindWidgetStack(WidgetStackTag);
			CHECK_NULL_RETURN(FoundWidgetStack);
			
			UWidgetActivatableBase* WidgetCreated = 
				FoundWidgetStack->AddWidget<UWidgetActivatableBase>(LoadedClass, 
					[AsyncPushCallback](UWidgetActivatableBase& WidgetCreatedBeforePush)
			{
				AsyncPushCallback(EAsyncPushWidgetState::CreatedAndBeforePush, &WidgetCreatedBeforePush);
			});
			CHECK_NULL_RETURN(WidgetCreated);
			AsyncPushCallback(EAsyncPushWidgetState::CreatedAndPushed, WidgetCreated);
			PrintInLog(FString::Format(TEXT("Successfully pushed widget {0} to stack asynchronously."),
				{WidgetCreated->GetName()}), Display);
		}));
}

void UUIGameInstanceSubsystem::PushConfirmWidgetToModalStackAsync(
	const EConfirmScreenType ConfirmScreenType,
	const FText& InWidgetTitle, 
	const FText& InWidgetMessage, 
	const TFunction<void(EConfirmScreenButtonType)>& ConfirmCallback)
{
	CHECK_NULL_RETURN(ConfirmCallback);
	UConfirmWidgetInfoObject* ConfirmWidgetInfoObject = 
		UConfirmWidgetInfoObject::CreateConfirmWidget(InWidgetTitle, InWidgetMessage, ConfirmScreenType);
	CHECK_NULL_RETURN(ConfirmWidgetInfoObject);

	PushWidgetSoftPtrToStackAsync(UUIFunctionLibrary::GetWidgetSoftFromSettings(
		FrontEndGameplayTags::FrontEnd_Widget_Confirmation),FrontEndGameplayTags::FrontEnd_WidgetStack_Modal,
		[ConfirmCallback, ConfirmWidgetInfoObject](
			const EAsyncPushWidgetState PushWidgetState, UWidgetActivatableBase* InWidget)
		{
			if (PushWidgetState == EAsyncPushWidgetState::CreatedAndBeforePush)
			{
				UWidgetConfirmation* ConfirmWidget = Cast<UWidgetConfirmation>(InWidget);
				CHECK_NULL_RETURN(ConfirmWidget);
				ConfirmWidget->InitializeConfirmWidget(ConfirmWidgetInfoObject, ConfirmCallback);
			}
		});
}

