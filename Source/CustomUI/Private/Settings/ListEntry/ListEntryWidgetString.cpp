// Amigo Luis all rights reserved


#include "Settings/ListEntry/ListEntryWidgetString.h"

#include "CheckAndLogAndReturn.h"
#include "CommonInputSubsystem.h"
#include "LogMacros.h"
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
	SettingEntryValueRotator->OnRotatedEvent.AddUObject(this, &UListEntryWidgetString::OnRotatorValueChanged);
}

void UListEntryWidgetString::OnOwningListItemObjectModified(UListSettingDataObjectBase* ModifiedData,
	ESettingsListDataModifyReason ModifyReason)
{
	Super::OnOwningListItemObjectModified(ModifiedData, ModifyReason);
	CHECK_NULL_RETURN(OwningListItemObject);
	SettingEntryValueRotator->SetSelectedItemByText(OwningListItemObject->GetCurrentSettingNameText());
}

void UListEntryWidgetString::OnToggleEditableState(const bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);
	IncreaseValueButton->SetIsEnabled(bIsEditable);
	SettingEntryValueRotator->SetIsEnabled(bIsEditable);
	DecreaseValueButton->SetIsEnabled(bIsEditable);
}

void UListEntryWidgetString::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	DecreaseValueButton->OnClicked().AddUObject(this, &UListEntryWidgetString::OnDecreaseValueButtonClicked);
	IncreaseValueButton->OnClicked().AddUObject(this, &UListEntryWidgetString::OnIncreaseValueButtonClicked);
	SettingEntryValueRotator->OnClicked().AddUObject(this, &UListEntryWidgetString::SelectThisEntryWidget);
}

void UListEntryWidgetString::OnDecreaseValueButtonClicked()
{
	LOG_ENTER_FUNCTION();
	if (OwningListItemObject != nullptr)
	{
		OwningListItemObject->ToPreviousStringAndText();
	}
	SelectThisEntryWidget();
}

void UListEntryWidgetString::OnIncreaseValueButtonClicked()
{
	LOG_ENTER_FUNCTION();
	if (OwningListItemObject != nullptr)
	{
		OwningListItemObject->ToNextStringAndText();
	}
	SelectThisEntryWidget();
}

void UListEntryWidgetString::OnRotatorValueChanged(int32 InValue, bool bUserInitiated)
{
	CHECK_NULL_RETURN(OwningListItemObject);
	CHECK_NULL_RETURN(SettingEntryValueRotator);
	CHECK_BOOL_FALSE_RETURN_WARNING(bUserInitiated);
	const UCommonInputSubsystem* InputSubsystem = GetInputSubsystem();
	CHECK_NULL_RETURN(InputSubsystem);
	
	if (InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		OwningListItemObject->OnTextChanged(SettingEntryValueRotator->GetSelectedText());
	}
}
