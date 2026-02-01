// Amigo Luis all rights reserved


#include "Widgets/WidgetKeyRemapConfirm.h"

#include "UILogger.h"
#include "FunctionLibraries/KeyRemapInputProcessor.h"

void UWidgetKeyRemapConfirm::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	CachedKeyRemapInputProcessor = MakeShared<FKeyRemapInputProcessor>(CurrentInputTypeToListen);
	CHECK_NULL_RETURN(CachedKeyRemapInputProcessor);
	FSlateApplication::Get().RegisterInputPreProcessor(CachedKeyRemapInputProcessor, -1);
}

void UWidgetKeyRemapConfirm::SetInputTypeToListen(const ECommonInputType InInputType)
{
	CurrentInputTypeToListen = InInputType;
}

void UWidgetKeyRemapConfirm::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	CHECK_NULL_RETURN(CachedKeyRemapInputProcessor);
	FSlateApplication::Get().UnregisterInputPreProcessor(CachedKeyRemapInputProcessor);
	CachedKeyRemapInputProcessor.Reset();
}

