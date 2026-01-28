// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectBase.h"

#include "Settings/FrontendGameUserSettings.h"
#include "Settings/DataObjects/SettingDataEditConditionDetail.h"

void UListSettingDataObjectBase::InitializeDataObject()
{
	OnInitializeDataObject();
}

void UListSettingDataObjectBase::AddEditCondition(const FSettingDataEditConditionDetail& SettingDataEditConditionDetail)
{
	SettingDataEditConditionDetails.Add(SettingDataEditConditionDetail);
}

bool UListSettingDataObjectBase::IsSettingDataEditable()
{
	bool bIsEditable = true;
	if (SettingDataEditConditionDetails.IsEmpty()) return bIsEditable;
	
	FString CachedDisabledRichReason;
	for (const FSettingDataEditConditionDetail& SettingDataEditCondition : SettingDataEditConditionDetails)
	{
		if (!SettingDataEditCondition.IsConditionValid() || SettingDataEditCondition.IsEditable()) continue;
		bIsEditable = false;
		CachedDisabledRichReason.Append(SettingDataEditCondition.GetDisabledRichReason());
		// TODO:确认下这里是否需要移除到循环外
		SetDisabledRichText(FText::FromString(CachedDisabledRichReason));
		if (SettingDataEditCondition.HasForcedStringValue())
		{
			const FString& ForcedString = SettingDataEditCondition.GetDisabledForcedStringValue();
			if (CanSetToForcedStringValue(ForcedString))
			{
				OnSetToForcedStringValue(ForcedString);
			}
		}
	}
	return bIsEditable;
}

void UListSettingDataObjectBase::OnInitializeDataObject()
{
}

void UListSettingDataObjectBase::NotifyListDataModified(UListSettingDataObjectBase* ModifiedData,
	const ESettingsListDataModifyReason ModifyReason)
{
	OnListDataModifiedDelegate.Broadcast(ModifiedData, ModifyReason);
	if (bShouldApplySettingChangeImmediately)
	{
		UFrontendGameUserSettings::Get()->ApplySettings(true);
	}
}
