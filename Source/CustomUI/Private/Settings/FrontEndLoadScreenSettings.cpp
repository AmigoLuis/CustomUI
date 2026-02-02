// Amigo Luis all rights reserved


#include "Settings/FrontEndLoadScreenSettings.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "UILogger.h"

TSubclassOf<UUserWidget> UFrontEndLoadScreenSettings::GetLoadingScreenWidgetClass() const
{
	CHECK_BOOL_TRUE_RETURN_VALUE_WARN(SoftLoadingScreenWidgetClass.IsNull(), TSubclassOf<UUserWidget>());
	return SoftLoadingScreenWidgetClass.LoadSynchronous();
}
