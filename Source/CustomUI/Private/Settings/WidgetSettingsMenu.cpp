// Amigo Luis all rights reserved


#include "Settings/WidgetSettingsMenu.h"

#include "ICommonInputModule.h"
#include "UILogger.h"
#include "Widgets/FrontEndButtonBase.h"
#include "Input/CommonUIInputTypes.h"

void UWidgetSettingsMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ResetActionHandle = RegisterUIActionBinding(FBindUIActionArgs(
		ICommonInputModule::GetSettings().GetDefaultBackAction(), true,
		FSimpleDelegate::CreateUObject(
			this, 
			&UWidgetSettingsMenu::OnBackActionTriggeredInSettingsMenu)));

	CHECK_BOOL_TRUE_RETURN_WARN(ResetAction.IsNull());
	ResetActionHandle = RegisterUIActionBinding(FBindUIActionArgs(
		ResetAction, true,
		FSimpleDelegate::CreateUObject(
			this, 
			&UWidgetSettingsMenu::OnResetActionTriggeredInSettingsMenu)));
}

void UWidgetSettingsMenu::OnResetActionTriggeredInSettingsMenu()
{
	PrintInLog(TEXT("Reset Action Triggered from Settings Menu"), Display);
}

void UWidgetSettingsMenu::OnBackActionTriggeredInSettingsMenu()
{
	PrintInLog(TEXT("Back Action Triggered from Settings Menu"), Display);
	DeactivateWidget();
}
