// Amigo Luis all rights reserved


#include "AsyncActions/AsyncPushConfirmationWidget.h"

#include "CheckAndLogAndReturn.h"
#include "UILogger.h"
#include "Subsystems/UIGameInstanceSubsystem.h"

UAsyncPushConfirmationWidget* UAsyncPushConfirmationWidget::PushConfirmWidgetToStackAsync(
	const UObject* WorldContextObject, const EConfirmScreenType ConfirmScreenType, const FText& InTitleText,
	const FText& InMessageText)
{
	CHECK_NULL_RETURN_VALUE(GEngine, nullptr);
	
	UWorld* World = 
		GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	CHECK_NULL_RETURN_VALUE(World, nullptr);
	
	UAsyncPushConfirmationWidget* Node = NewObject<UAsyncPushConfirmationWidget>();
	CHECK_NULL_RETURN_VALUE(Node, nullptr);
	Node->CachedOwningWorld = World;
	Node->CachedConfirmScreenType = ConfirmScreenType;
	Node->CachedTitleText = InTitleText;
	Node->CachedMessageText = InMessageText;
	Node->RegisterWithGameInstance(World);
	return Node;
}

void UAsyncPushConfirmationWidget::Activate()
{
	Super::Activate();
	UUIGameInstanceSubsystem* UIGameInstanceSubsystem = 
		UUIGameInstanceSubsystem::Get(CachedOwningWorld.Get());
	CHECK_NULL_RETURN(UIGameInstanceSubsystem)
	
	UIGameInstanceSubsystem->PushConfirmWidgetToModalStackAsync(
		CachedConfirmScreenType,
		CachedTitleText,
		CachedMessageText,
		[this](EConfirmScreenButtonType ConfirmScreenButtonType)
		{
			OnConfirmWidgetButtonClicked.Broadcast(ConfirmScreenButtonType);
			SetReadyToDestroy();
		});
}
