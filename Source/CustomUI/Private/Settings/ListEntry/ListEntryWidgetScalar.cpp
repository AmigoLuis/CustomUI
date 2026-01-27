// Amigo Luis all rights reserved


#include "Settings/ListEntry/ListEntryWidgetScalar.h"

void UListEntryWidgetScalar::OnListItemObjectSet(UListSettingDataObjectBase* InOwningListItemObject)
{
	Super::OnListItemObjectSet(InOwningListItemObject);
}

void UListEntryWidgetScalar::OnOwningListItemObjectModified(UListSettingDataObjectBase* ModifiedData,
	ESettingsListDataModifyReason ModifyReason)
{
	Super::OnOwningListItemObjectModified(ModifiedData, ModifyReason);
}

void UListEntryWidgetScalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}
