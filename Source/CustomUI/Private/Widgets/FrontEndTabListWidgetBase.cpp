// Amigo Luis all rights reserved


#include "Widgets/FrontEndTabListWidgetBase.h"

#include "UILogger.h"
#include "Editor/WidgetCompilerLog.h"
#include "Widgets/FrontEndButtonBase.h"

#if WITH_EDITOR
M_VALIDATE_COMPILED_DEFAULTS_DEFINE(UFrontEndTabListWidgetBase, TabButtonEntryClass, UFrontEndButtonBase)
#endif


void UFrontEndTabListWidgetBase::RequestRegisterTab(const FName& InTabId, const FText& InTabDisplayName)
{
	RegisterTab(InTabId, TabButtonEntryClass, nullptr);
	
	UFrontEndButtonBase* FoundButton = Cast<UFrontEndButtonBase>(GetTabButtonBaseByID(InTabId));
	CHECK_NULL_RETURN_WARN(FoundButton);
	FoundButton->SetButtonText(InTabDisplayName);
}

