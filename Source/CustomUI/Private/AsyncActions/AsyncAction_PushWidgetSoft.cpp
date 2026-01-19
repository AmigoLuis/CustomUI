// Amigo Luis all rights reserved


#include "AsyncActions/AsyncAction_PushWidgetSoft.h"

#include "UILogger.h"
#include "GameplayTagContainer.h"

UAsyncAction_PushWidgetSoft* UAsyncAction_PushWidgetSoft::PushWidgetSoftToStackAsync(
	const UObject* WorldContextObject,
	APlayerController* PlayerController, 
	TSoftClassPtr<UWidgetActivatableBase> InWidgetSoftPtr,
	FGameplayTag InWidgetTag, 
	bool bFocusOnNewlyPushedWidget)
{
	if (InWidgetSoftPtr.IsNull()) {LOG_NULL_PTR(InWidgetSoftPtr); return nullptr;}
	CHECK_NULL_RETURN_VALUE(GEngine, nullptr);
	
	const UWorld* World = 
		GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	CHECK_NULL_RETURN_VALUE(World, nullptr);
	
	UAsyncAction_PushWidgetSoft* Node = NewObject<UAsyncAction_PushWidgetSoft>();
	CHECK_NULL_RETURN_VALUE(Node, nullptr);
	Node->RegisterWithGameInstance(GEngine);
	return Node;
}
