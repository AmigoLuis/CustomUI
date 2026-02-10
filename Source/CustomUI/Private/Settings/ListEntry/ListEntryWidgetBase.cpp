// Amigo Luis all rights reserved


#include "Settings/ListEntry/ListEntryWidgetBase.h"

#include "CheckAndLogAndReturn.h"
#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "UILogger.h"
#include "Components/ListView.h"
#include "Settings/DataObjects/ListSettingDataObjectBase.h"

void UListEntryWidgetBase::NativeOnListEntryWidgetHovered(const bool bIsHovered)
{
	BP_OnListEntryWidgetHovered(bIsHovered, GetListItem() == nullptr ? false : IsListItemSelected());
	if (bIsHovered)
	{
		BP_OnToggleEntryWidgetHighlightState(true);
	}
	else
	{
		BP_OnToggleEntryWidgetHighlightState(GetListItem() != nullptr && IsListItemSelected() ? true : false);
	}
}

void UListEntryWidgetBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
	OnListItemObjectSet(Cast<UListSettingDataObjectBase>(ListItemObject));
}

void UListEntryWidgetBase::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();
	NativeOnListEntryWidgetHovered(false);
}

void UListEntryWidgetBase::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
	BP_OnToggleEntryWidgetHighlightState(bIsSelected);
	if (bIsSelected)
	{
		// 核心逻辑：一旦我被选中了，我立刻检查自己是否有焦点
		// 如果没有，强制抢过来。这解决了循环导航后“高亮在 A，输入还在 B”的问题
		if (!HasUserFocus(GetOwningPlayer()))
		{
			SetFocus(); 
		}
	}
}

FReply UListEntryWidgetBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
#undef LOCAL_DEFAULT_VALUE
#define LOCAL_DEFAULT_VALUE Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	
	const UCommonInputSubsystem* InputSubsystem = GetInputSubsystem();
	CHECK_NULL_RETURN_VALUE(InputSubsystem, LOCAL_DEFAULT_VALUE);

	const UWidget* const WidgetToFocus = BP_GetWidgetToFocusForGamepad();
	CHECK_NULL_RETURN_VALUE(WidgetToFocus, LOCAL_DEFAULT_VALUE);

	const TSharedPtr<SWidget> CachedWidgetToFocus = WidgetToFocus->GetCachedWidget();
	CHECK_BOOL_FALSE_RETURN_VALUE_WARNING(CachedWidgetToFocus.IsValid(), LOCAL_DEFAULT_VALUE);

	return FReply::Handled().SetUserFocus(CachedWidgetToFocus.ToSharedRef());

#undef LOCAL_DEFAULT_VALUE
}

void UListEntryWidgetBase::OnListItemObjectSet(UListSettingDataObjectBase* InOwningListItemObject)
{
	CHECK_NULL_RETURN(InOwningListItemObject);
	if (SettingEntryNameTextBlock)
	{
		SettingEntryNameTextBlock->SetText(InOwningListItemObject->GetDataDisplayName());
	}
	if (!InOwningListItemObject->OnListDataModifiedDelegate.IsBoundToObject(this))
	{
		InOwningListItemObject->OnListDataModifiedDelegate.AddUObject(this, 
			&UListEntryWidgetBase::OnOwningListItemObjectModified);	
	}
	
	if (!InOwningListItemObject->OnDependencyDataModifiedDelegate.IsBoundToObject(this))
	{
		InOwningListItemObject->OnDependencyDataModifiedDelegate.AddUObject(this, 
			&UListEntryWidgetBase::OnDependencyDataObjectModified);	
	}
	OnToggleEditableState(InOwningListItemObject->IsSettingDataEditable());
	CachedOwningListItemObject = InOwningListItemObject;
}

void UListEntryWidgetBase::OnOwningListItemObjectModified(UListSettingDataObjectBase* ModifiedData,
	ESettingsListDataModifyReason ModifyReason)
{
}

void UListEntryWidgetBase::OnDependencyDataObjectModified(UListSettingDataObjectBase* ModifiedData,
	ESettingsListDataModifyReason ModifyReason)
{
	if (CachedOwningListItemObject)
	{
		OnToggleEditableState(CachedOwningListItemObject->IsSettingDataEditable());
	}
}

void UListEntryWidgetBase::OnToggleEditableState(const bool bIsEditable)
{
	if (SettingEntryNameTextBlock)
	{
		SettingEntryNameTextBlock->SetIsEnabled(bIsEditable);
	}
}

void UListEntryWidgetBase::SelectThisEntryWidget()
{
	UListView* OwningListView = Cast<UListView>(GetOwningListView());
	CHECK_NULL_RETURN(OwningListView);
	OwningListView->SetSelectedItem(GetListItem());
}
