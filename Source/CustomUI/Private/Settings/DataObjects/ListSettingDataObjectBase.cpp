// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectBase.h"

void UListSettingDataObjectBase::InitializeDataObject()
{
	OnInitializeDataObject();
}

void UListSettingDataObjectBase::OnInitializeDataObject()
{
}

void UListSettingDataObjectBase::NotifyListDataModified(UListSettingDataObjectBase* ModifiedData,
	const ESettingsListDataModifyReason ModifyReason)
{
	OnListDataModifiedDelegate.Broadcast(ModifiedData, ModifyReason);
}
