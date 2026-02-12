// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectInteger.h"

#include "Settings/FSettingDataInteractionHelper.h"
#include "Widgets/StringTableLocations.h"

void UListSettingDataObjectInteger::AddIntegerSetting(int32 Value, const FText& InDisplayText)
{
	AddSettingEntry(LexToString(Value), InDisplayText);
}

void UListSettingDataObjectInteger::OnInitializeDataObject()
{
	Super::OnInitializeDataObject();
	if (!TrySetTextAccordingToString(CurrentSettingNameString))
	{
		CurrentSettingNameText = GET_VIDEO_SETTING_FOR_KEY("CustomVideoSettingDescription");
	}
}

void UListSettingDataObjectInteger::OnEditDependencyDataModified(UListSettingDataObjectBase* ModifiedData,
	ESettingsListDataModifyReason ModifyReason)
{
	if (DataDynamicGetter)
	{
		// 防止无限调用
		if (DataDynamicGetter->GetValueAsString() == CurrentSettingNameString) return;
		
		CurrentSettingNameString = DataDynamicGetter->GetValueAsString();
	
		if (!TrySetTextAccordingToString(CurrentSettingNameString))
		{
			CurrentSettingNameText = GET_VIDEO_SETTING_FOR_KEY("CustomVideoSettingDescription");
		}
		NotifyListDataModified(this, ESettingsListDataModifyReason::DependencyModified);
	}
	Super::OnEditDependencyDataModified(ModifiedData, ModifyReason);
}
