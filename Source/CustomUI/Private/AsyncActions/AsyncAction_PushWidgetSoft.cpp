// Amigo Luis all rights reserved


#include "AsyncActions/AsyncAction_PushWidgetSoft.h"

#include "UILogger.h"
#include "Subsystems/UIGameInstanceSubsystem.h"
#include "Widgets/WidgetActivatableBase.h"

UAsyncAction_PushWidgetSoft* UAsyncAction_PushWidgetSoft::PushWidgetSoftToStackAsync(
	const UObject* WorldContextObject,
	APlayerController* PlayerController,
	const TSoftClassPtr<UWidgetActivatableBase> InWidgetSoftPtr,
	const FGameplayTag InWidgetStackTag,
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
	Node->CachedWidgetStackTag = InWidgetStackTag;
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
		CachedWidgetStackTag,
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
				else 
					PrintInLog(SYMBOL_NAME_TEXT(WidgetToPush) TEXT("'s name is ") + 
						WidgetToPush->GetName() + 
						TEXT(", and ") SYMBOL_NAME_TEXT(WidgetToPush->GetDesiredFocusTarget()) 
						TEXT(" is nullptr in function:") TEXT(__FUNCTION__) 
						TEXT(", line: ") INT_TO_STR(__LINE__) TEXT("."));
				SetReadyToDestroy();
				break;
			default:
				break;
			}
		});
}
