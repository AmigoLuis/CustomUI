// Amigo Luis all rights reserved


#include "Settings/WidgetSettingsMenu.h"

#include "ICommonInputModule.h"
#include "UILogger.h"
#include "Widgets/FrontEndButtonBase.h"
#include "Input/CommonUIInputTypes.h"
#include "Settings/SettingDataRegistry.h"
#include "Settings/DataObjects/ListSettingDataObjectCollection.h"
#include "Widgets/FrontEndTabListWidgetBase.h"

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

void UWidgetSettingsMenu::NativeOnActivated()
{
	Super::NativeOnActivated();
	GetOrCreateSettingsDataRegistry();
	CHECK_NULL_RETURN(SettingsDataRegistry);
	const auto& TabCollections = 
		SettingsDataRegistry->GetRegisteredSettingTabCollections();
	for (UListSettingDataObjectCollection* TabCollection : TabCollections)
	{
		if (TabCollection == nullptr) continue;
		const FName& TabDataID = TabCollection->GetDataID();
		if (SettingsTabList->GetTabButtonBaseByID(TabDataID) != nullptr) continue;// Already created
		SettingsTabList->RequestRegisterTab(TabDataID, TabCollection->GetDataDisplayName());
	}
	
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

USettingDataRegistry* UWidgetSettingsMenu::GetOrCreateSettingsDataRegistry()
{
	if (!SettingsDataRegistry)
	{
		SettingsDataRegistry = NewObject<USettingDataRegistry>();
		SettingsDataRegistry->InitSettingDataRegistry(GetOwningLocalPlayer());
	}
	CHECK_NULL_RETURN_VALUE(SettingsDataRegistry, nullptr);
	return SettingsDataRegistry;
}
