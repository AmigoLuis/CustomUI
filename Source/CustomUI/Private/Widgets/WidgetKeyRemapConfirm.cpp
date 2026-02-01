// Amigo Luis all rights reserved


#include "Widgets/WidgetKeyRemapConfirm.h"

#include "UILogger.h"
#include "FunctionLibraries/KeyRemapInputProcessor.h"

void UWidgetKeyRemapConfirm::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	CachedKeyRemapInputProcessor = MakeShared<FKeyRemapInputProcessor>();
	CHECK_NULL_RETURN(CachedKeyRemapInputProcessor);
	FSlateApplication::Get().RegisterInputPreProcessor(CachedKeyRemapInputProcessor, -1);
}

void UWidgetKeyRemapConfirm::NativeOnActivated()
{
	Super::NativeOnActivated();
	CHECK_NULL_RETURN(CachedKeyRemapInputProcessor);
	FSlateApplication::Get().UnregisterInputPreProcessor(CachedKeyRemapInputProcessor);
	CachedKeyRemapInputProcessor.Reset();
}
