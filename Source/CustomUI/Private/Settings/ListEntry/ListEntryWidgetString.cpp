// Amigo Luis all rights reserved


#include "Settings/ListEntry/ListEntryWidgetString.h"

#include "UILogger.h"
#include "Settings/DataObjects/ListSettingDataObjectString.h"
#include "Widgets/Components/FrontEndButtonBase.h"
#include "Widgets/Components/FrontEndCommonRotator.h"

void UListEntryWidgetString::OnListItemObjectSet(UListSettingDataObjectBase* InOwningListItemObject)
{
	Super::OnListItemObjectSet(InOwningListItemObject);
	OwningListItemObject = Cast<UListSettingDataObjectString>(InOwningListItemObject);
	CHECK_NULL_RETURN(OwningListItemObject);
	
	SettingEntryValueRotator->PopulateTextLabels(OwningListItemObject->GetAllSettingNameText());
	SettingEntryValueRotator->SetSelectedItemByText(OwningListItemObject->GetCurrentSettingNameText());
}

void UListEntryWidgetString::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	DecreaseValueButton->OnClicked().AddUObject(this, &UListEntryWidgetString::OnDecreaseValueButtonClicked);
	IncreaseValueButton->OnClicked().AddUObject(this, &UListEntryWidgetString::OnIncreaseValueButtonClicked);
}

void UListEntryWidgetString::OnDecreaseValueButtonClicked()
{
	LOG_ENTER_FUNCTION();
}

void UListEntryWidgetString::OnIncreaseValueButtonClicked()
{
	LOG_ENTER_FUNCTION();
}
