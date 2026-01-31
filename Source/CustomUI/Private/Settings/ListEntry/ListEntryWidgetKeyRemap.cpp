// Amigo Luis all rights reserved


#include "Settings/ListEntry/ListEntryWidgetKeyRemap.h"

#include "UILogger.h"
#include "Settings/DataObjects/ListSettingDataObjectKeyRemap.h"
#include "Widgets/Components/FrontEndButtonBase.h"

void UListEntryWidgetKeyRemap::OnListItemObjectSet(UListSettingDataObjectBase* InOwningListItemObject)
{
	Super::OnListItemObjectSet(InOwningListItemObject);
	OwningKeyRemapObject = Cast<UListSettingDataObjectKeyRemap>(InOwningListItemObject);
	CHECK_NULL_RETURN(OwningKeyRemapObject);
	RemapKeyButton->SetButtonDisplayImageBrush(OwningKeyRemapObject->GetSlateBrushForCurrentKey());
}

void UListEntryWidgetKeyRemap::OnOwningListItemObjectModified(UListSettingDataObjectBase* ModifiedData,
	ESettingsListDataModifyReason ModifyReason)
{
	Super::OnOwningListItemObjectModified(ModifiedData, ModifyReason);
	CHECK_NULL_RETURN(OwningKeyRemapObject);
	RemapKeyButton->SetButtonDisplayImageBrush(OwningKeyRemapObject->GetSlateBrushForCurrentKey());
}
