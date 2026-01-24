// Amigo Luis all rights reserved


#include "Settings/ListEntry/ListEntryWidgetBase.h"

#include "CommonTextBlock.h"
#include "UILogger.h"
#include "Settings/DataObjects/ListSettingDataObjectBase.h"

void UListEntryWidgetBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	OnListItemObjectSet(Cast<UListSettingDataObjectBase>(ListItemObject));
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
}

void UListEntryWidgetBase::OnOwningListItemObjectModified(UListSettingDataObjectBase* ModifiedData,
	ESettingsListDataModifyReason ModifyReason)
{
}
