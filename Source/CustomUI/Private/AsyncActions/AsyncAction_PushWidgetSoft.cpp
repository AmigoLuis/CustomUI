// Amigo Luis all rights reserved


#include "AsyncActions/AsyncAction_PushWidgetSoft.h"

#include "UILogger.h"
#include "Subsystems/UIGameInstanceSubsystem.h"
#include "Widgets/WidgetActivatableBase.h"

UAsyncAction_PushWidgetSoft* UAsyncAction_PushWidgetSoft::PushWidgetSoftToStackAsync(
	const UObject* WorldContextObject,
	APlayerController* PlayerController,
	const TSoftClassPtr<UWidgetActivatableBase> InWidgetSoftPtr,
	const FGameplayTag InWidgetTag,
	const bool bFocusOnNewlyPushedWidget)
{
	if (InWidgetSoftPtr.IsNull()) {LOG_NULL_PTR(InWidgetSoftPtr); return nullptr;}
	CHECK_NULL_RETURN_VALUE(GEngine, nullptr);
	
	UWorld* World = 
		GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	CHECK_NULL_RETURN_VALUE(World, nullptr);
	
	UAsyncAction_PushWidgetSoft* Node = NewObject<UAsyncAction_PushWidgetSoft>();
	CHECK_NULL_RETURN_VALUE(Node, nullptr);
	Node->CachedOwningWorld = World;
	Node->CachedOwningPlayerController = PlayerController;
	Node->CachedWidgetSoftPtr = InWidgetSoftPtr;
	Node->CachedWidgetTag = InWidgetTag;
	Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;
	
	Node->RegisterWithGameInstance(World);
	return Node;
}

void UAsyncAction_PushWidgetSoft::Activate()
{
	LOG_ENTER_FUNCTION();
	UUIGameInstanceSubsystem* UIGameInstanceSubsystem = 
		UUIGameInstanceSubsystem::Get(CachedOwningWorld.Get());
	CHECK_NULL_RETURN(UIGameInstanceSubsystem)
	
	UIGameInstanceSubsystem->PushWidgetSoftPtrToStackAsync(
		CachedWidgetSoftPtr,
		CachedWidgetTag,
		[this](const EAsyncPushWidgetState InPushState, UWidgetActivatableBase* WidgetToPush)
		{
			LOG_ENTER_FUNCTION();
			CHECK_NULL_RETURN(WidgetToPush);
			switch (InPushState)
			{
			case EAsyncPushWidgetState::CreatedAndBeforePush:
				WidgetToPush->SetOwningPlayer(CachedOwningPlayerController.Get());
				OnWidgetCreatedAndBeforePush.Broadcast(WidgetToPush);
				break;
			case EAsyncPushWidgetState::CreatedAndPushed:
				OnWidgetCreatedAndPushed.Broadcast(WidgetToPush);
				if (UWidget* WidgetToFocus = 
					bCachedFocusOnNewlyPushedWidget ? WidgetToPush->GetDesiredFocusTarget() : nullptr) 
						WidgetToFocus->SetFocus();
				SetReadyToDestroy();
				break;
			default:
				break;
			}
		});
}
