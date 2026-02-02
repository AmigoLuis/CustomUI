// Amigo Luis all rights reserved


#include "Widgets/WidgetSettingsMenu.h"

#include "ICommonInputModule.h"
#include "UILogger.h"
#include "Widgets/Components/FrontEndButtonBase.h"
#include "Input/CommonUIInputTypes.h"
#include "Settings/FrontendGameUserSettings.h"
#include "Settings/SettingDataRegistry.h"
#include "Settings/WidgetSettingDetailsView.h"
#include "Settings/DataObjects/ListSettingDataObjectCollection.h"
#include "Settings/ListEntry/ListEntryWidgetBase.h"
#include "Subsystems/UIGameInstanceSubsystem.h"
#include "Widgets/Components/FrontEndCommonListView.h"
#include "Widgets/Components/FrontEndTabListWidgetBase.h"

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
	
	SettingsTabList->OnTabSelected.AddUniqueDynamic(
		this, &UWidgetSettingsMenu::OnTabSelectedInSettingsMenu);
	
	SettingsListView->OnItemIsHoveredChanged().AddUObject(
		this, &UWidgetSettingsMenu::OnListViewItemHovered);
	
	SettingsListView->OnItemSelectionChanged().AddUObject(
		this, &UWidgetSettingsMenu::OnListViewItemSelectionChanged);
}

void UWidgetSettingsMenu::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	UFrontendGameUserSettings::Get()->ApplySettings(true);
}

void UWidgetSettingsMenu::NativeOnActivated()
{
	Super::NativeOnActivated();
	GetOrCreateSettingsDataRegistry();
	CHECK_NULL_RETURN(SettingsDataRegistry);
	const auto& TabCollections = 
		SettingsDataRegistry->GetRegisteredSettingTabCollections();
	for (const UListSettingDataObjectCollection* TabCollection : TabCollections)
	{
		if (TabCollection == nullptr) continue;
		const FName& TabDataID = TabCollection->GetDataID();
		if (SettingsTabList->GetTabButtonBaseByID(TabDataID) != nullptr) continue;// Already created
		SettingsTabList->RequestRegisterTab(TabDataID, TabCollection->GetDataDisplayName());
	}
	
}

UWidget* UWidgetSettingsMenu::NativeGetDesiredFocusTarget() const
{
	const UObject* SelectedObject = SettingsListView->GetSelectedItem();
	CHECK_NULL_RETURN_VALUE(SelectedObject, Super::NativeGetDesiredFocusTarget());
	UUserWidget* SelectedEntryWidget = SettingsListView->GetEntryWidgetFromItem(SelectedObject);
	CHECK_NULL_RETURN_VALUE(SelectedEntryWidget, Super::NativeGetDesiredFocusTarget());
	return SelectedEntryWidget;
}

void UWidgetSettingsMenu::OnResetActionTriggeredInSettingsMenu()
{
	PrintInLog(TEXT("Reset Action Triggered from Settings Menu"), Display);
	CHECK_BOOL_TRUE_RETURN_WARN(ResettableDataArray.IsEmpty());

	const UFrontEndButtonBase* TabButtonBase = 
		Cast<UFrontEndButtonBase>(SettingsTabList->GetTabButtonBaseByID(SettingsTabList->GetActiveTab()));
	CHECK_NULL_RETURN(TabButtonBase);
	
	UUIGameInstanceSubsystem::Get(this)->PushConfirmWidgetToModalStackAsync(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("Reset")),
		FText::FromString(TEXT("Are you sure you want to reset all settings in this ")
			+ TabButtonBase->GetButtonText().ToString() 
			+ TEXT(" tab to default values?")),[this](const EConfirmScreenButtonType ConfirmScreenButtonType)
			{
				if (ConfirmScreenButtonType != EConfirmScreenButtonType::Confirmed)
				{
					PrintInLog(TEXT("Reset Action Not Confirmed"), Display);
					return;
				}
				this->bIsResettingData = true;
				bool bIsAnyDataFailedToReset = false;
				for (UListSettingDataObjectBase* ResettableData : ResettableDataArray)
				{
					if (ResettableData == nullptr) continue;
					if (ResettableData->TryResetToDefaultValue())
					{
						PrintInLog(ResettableData->GetDataDisplayName().ToString() + TEXT(" was reset."), Display);
					}
					else
					{
						bIsAnyDataFailedToReset = true;
						PrintInLog(ResettableData->GetDataDisplayName().ToString() + TEXT(" was failed to reset."));
					}
				}
				if (!bIsAnyDataFailedToReset)
				{
					ResettableDataArray.Empty();
					RemoveActionBinding(ResetActionHandle);
				}
				this->bIsResettingData = false;
			});
	
}

void UWidgetSettingsMenu::OnBackActionTriggeredInSettingsMenu()
{
	PrintInLog(TEXT("Back Action Triggered from Settings Menu"), Display);
	DeactivateWidget();
}

FString UWidgetSettingsMenu::TryGetEntryWidgetClassName(UObject* InOwningListItem) const
{
	const UUserWidget* FoundWidget = SettingsListView->GetEntryWidgetFromItem(InOwningListItem);
	CHECK_NULL_RETURN_VALUE_WARN(FoundWidget, FString(TEXT("InvalidEntryWidget")));
	return FoundWidget->GetClass()->GetName();
}

void UWidgetSettingsMenu::OnListDataModified(UListSettingDataObjectBase* InModifiedData,
	ESettingsListDataModifyReason InModifiedReason)
{
	CHECK_NULL_RETURN(InModifiedData);
	if (bIsResettingData) return; //正在重置数据时，不处理数据修改回调
	if (InModifiedData->CanResetToDefaultValue())
	{
		ResettableDataArray.AddUnique(InModifiedData);
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	else if (ResettableDataArray.Contains(InModifiedData)) //不能重置为默认值，需要从可重置数据中剔除
	{
		ResettableDataArray.Remove(InModifiedData);
	}
	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
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

void UWidgetSettingsMenu::OnTabSelectedInSettingsMenu(const FName TabID)
{
	PrintInLog(TEXT("Tab Selected from Settings Menu, Tab id:") + TabID.ToString(), Display);
	
	SettingDetailsView->ClearDetailViewInfo();
	const auto& FoundListSourceItems = 
		GetOrCreateSettingsDataRegistry()->GetListSourceItemsBySelectedTabId(TabID);
	
	SettingsListView->SetListItems(FoundListSourceItems);
	SettingsListView->RequestRefresh();
	if (SettingsListView->GetNumItems() != 0)
	{
		SettingsListView->NavigateToIndex(0);
		SettingsListView->SetSelectedIndex(0);
	}
	
	ResettableDataArray.Empty();
	for (auto FoundListSourceItem: FoundListSourceItems)
	{
		if (!FoundListSourceItem) continue;
		
		if (!FoundListSourceItem->OnListDataModifiedDelegate.IsBoundToObject(this))
		{
			FoundListSourceItem->OnListDataModifiedDelegate.AddUObject(this, &UWidgetSettingsMenu::OnListDataModified);
		} 
		
		if (FoundListSourceItem->CanResetToDefaultValue())
		{
			ResettableDataArray.AddUnique(FoundListSourceItem);
		}
	}
	
	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);	
	}
	else if (!GetActionBindings().Contains(ResetActionHandle))
	{
		AddActionBinding(ResetActionHandle);
	}
}

void UWidgetSettingsMenu::OnListViewItemHovered(UObject* InHoveredItem, bool bIsHovered)
{
	CHECK_NULL_RETURN_WARN(InHoveredItem);
	const UListSettingDataObjectBase* HoveredDataObject = 
		Cast<UListSettingDataObjectBase>(InHoveredItem);
	CHECK_NULL_RETURN_WARN(HoveredDataObject);
	
	const FString& DisplayName = HoveredDataObject->GetDataDisplayName().ToString();
	PrintInLog(DisplayName + TEXT("'s hover state is : ") + 
		(bIsHovered ? TEXT("hovered") : TEXT("unhovered")), VeryVerbose);
	UListEntryWidgetBase* HoveredEntryWidget = SettingsListView->GetEntryWidgetFromItem<
		UListEntryWidgetBase>(HoveredDataObject);
	CHECK_NULL_RETURN(HoveredEntryWidget)
	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bIsHovered);
	if (bIsHovered)
	{
		SettingDetailsView->UpdateDetailViewInfo(Cast<UListSettingDataObjectBase>(InHoveredItem), 
			TryGetEntryWidgetClassName(InHoveredItem));
	}
	else
	{
		UListSettingDataObjectBase* SettingDataObject = 
			SettingsListView->GetSelectedItem<UListSettingDataObjectBase>();
		CHECK_NULL_RETURN(SettingDataObject);
		SettingDetailsView->UpdateDetailViewInfo(SettingDataObject, 
			TryGetEntryWidgetClassName(SettingDataObject));
	}
}

void UWidgetSettingsMenu::OnListViewItemSelectionChanged(UObject* InSelectedItem)
{
	CHECK_NULL_RETURN_WARN(InSelectedItem);
	const UListSettingDataObjectBase* SelectedDataObject = 
		Cast<UListSettingDataObjectBase>(InSelectedItem);
	CHECK_NULL_RETURN_WARN(SelectedDataObject);
	
	const FString& DisplayName = SelectedDataObject->GetDataDisplayName().ToString();
	PrintInLog(DisplayName + TEXT("'s hover state is selected"), VeryVerbose);
	
	SettingDetailsView->UpdateDetailViewInfo(Cast<UListSettingDataObjectBase>(InSelectedItem), 
		TryGetEntryWidgetClassName(InSelectedItem));
}
