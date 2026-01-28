// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectInteger.h"

#include "Settings/FSettingDataInteractionHelper.h"

void UListSettingDataObjectInteger::AddIntegerSetting(int32 Value, const FText& InDisplayText)
{
	AddSettingEntry(LexToString(Value), InDisplayText);
}

void UListSettingDataObjectInteger::OnInitializeDataObject()
{
	Super::OnInitializeDataObject();
	if (!TrySetTextAccordingToString(CurrentSettingNameString))
	{
		CurrentSettingNameText = FText::FromString(TEXT("Custom"));
	}
}

void UListSettingDataObjectInteger::OnEditDependencyDataModified(UListSettingDataObjectBase* ModifiedData,
	ESettingsListDataModifyReason ModifyReason)
{
	if (DataDynamicGetter)
	{
		CurrentSettingNameString = DataDynamicGetter->GetValueAsString();
	
		if (!TrySetTextAccordingToString(CurrentSettingNameString))
		{
			CurrentSettingNameText = FText::FromString(TEXT("Custom"));
		}
		NotifyListDataModified(this, ESettingsListDataModifyReason::DependencyModified);
	}
	Super::OnEditDependencyDataModified(ModifiedData, ModifyReason);
}
