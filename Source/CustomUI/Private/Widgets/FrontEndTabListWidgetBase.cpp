// Amigo Luis all rights reserved


#include "Widgets/FrontEndTabListWidgetBase.h"

#include "UILogger.h"
#include "Editor/WidgetCompilerLog.h"
#include "Widgets/FrontEndButtonBase.h"

#if WITH_EDITOR
void UFrontEndTabListWidgetBase::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	if (!TabButtonEntryClass)
	{
		const FString& ErrorMessage = FString::Format(TEXT("The variable {0} has not been set.\n"
			"Please set it to a valid subclass of {1}.\n"
			"{2} needs a valid {0} to function properly."), 
			{SYMBOL_NAME_TEXT(TabButtonEntryClass), 
				SYMBOL_NAME_TEXT(UFrontEndButtonBase), 
				GetClass()->GetName()});
		CompileLog.Error(FText::FromString(ErrorMessage));
	}
}
#endif


void UFrontEndTabListWidgetBase::RequestRegisterTab(const FName& InTabId, const FText& InTabDisplayName)
{
	
}

