// Amigo Luis all rights reserved


#include "Settings/ListEntry/ListEntryWidgetString.h"

#include "UILogger.h"
#include "Settings/DataObjects/ListSettingDataObjectString.h"
#include "Widgets/Components/FrontEndCommonRotator.h"

void UListEntryWidgetString::OnListItemObjectSet(UListSettingDataObjectBase* InOwningListItemObject)
{
	Super::OnListItemObjectSet(InOwningListItemObject);
	OwningListItemObject = Cast<UListSettingDataObjectString>(InOwningListItemObject);
	CHECK_NULL_RETURN(OwningListItemObject);
	
	SettingEntryValueRotator->PopulateTextLabels(OwningListItemObject->GetAllSettingNameText());
	SettingEntryValueRotator->SetSelectedItemByText(OwningListItemObject->GetCurrentSettingNameText());
}
