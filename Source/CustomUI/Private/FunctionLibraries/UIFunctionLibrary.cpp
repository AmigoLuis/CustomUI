// Amigo Luis all rights reserved


#include "FunctionLibraries/UIFunctionLibrary.h"

#include "UILogger.h"
#include "Settings/FrontEndDeveloperSettings.h"

TSoftClassPtr<UWidgetActivatableBase> UUIFunctionLibrary::GetWidgetSoftFromSettings(FGameplayTag WidgetTag)
{
	TSoftClassPtr<UWidgetActivatableBase> WidgetSoft;
	const UFrontEndDeveloperSettings* Settings = GetDefault<UFrontEndDeveloperSettings>();
	CHECK_NULL_RETURN_VALUE(Settings, WidgetSoft);
	if (!Settings->WidgetClassesMap.Contains(WidgetTag))
	{
		PrintInLog(TEXT("Can't find widget under the tag: ") + WidgetTag.ToString());
		return WidgetSoft;
	}
	return Settings->WidgetClassesMap.FindRef(WidgetTag);
}
