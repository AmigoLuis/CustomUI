// Amigo Luis all rights reserved


#include "AsyncActions/AsyncPushConfirmationWidget.h"

#include "CheckAndLogAndReturn.h"
#include "UILogger.h"
#include "Subsystems/UIGameInstanceSubsystem.h"
#include "Widgets/StringTableLocations.h"

UAsyncPushConfirmationWidget* UAsyncPushConfirmationWidget::PushConfirmWidgetToStackAsync(
	const UObject* WorldContextObject, const EConfirmScreenType ConfirmScreenType,
	const FString& InTitleTextKeyInSt, const FString& InMessageTextKeyInSt)
{
	return PushConfirmWidgetToStackAsyncWithFullParameter(
		WorldContextObject, ConfirmScreenType, InTitleTextKeyInSt,
		InMessageTextKeyInSt, TEXT(ST_UN_ASSORTED));
}

UAsyncPushConfirmationWidget* UAsyncPushConfirmationWidget::PushConfirmWidgetToStackAsyncWithFullParameter(
	const UObject* WorldContextObject, const EConfirmScreenType ConfirmScreenType,
	const FString& InTitleTextKeyInSt,
	const FString& InMessageTextKeyInSt, const FName& StringTableID)
{
	CHECK_NULL_RETURN_VALUE(GEngine, nullptr);
	
	UWorld* World = 
		GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	CHECK_NULL_RETURN_VALUE(World, nullptr);
	
	UAsyncPushConfirmationWidget* Node = NewObject<UAsyncPushConfirmationWidget>();
	CHECK_NULL_RETURN_VALUE(Node, nullptr);
	Node->CachedOwningWorld = World;
	Node->CachedConfirmScreenType = ConfirmScreenType;
	Node->CachedTitleText =
		GET_VALUE_FOR_KEY_FROM_ST_DIRECT(StringTableID, InTitleTextKeyInSt);
	Node->CachedMessageText =
		GET_VALUE_FOR_KEY_FROM_ST_DIRECT(StringTableID, InMessageTextKeyInSt);
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
