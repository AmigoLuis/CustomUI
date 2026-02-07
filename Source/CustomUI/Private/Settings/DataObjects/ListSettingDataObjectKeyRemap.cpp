// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectKeyRemap.h"

#include "CheckAndLogAndReturn.h"
#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"
#include "UILogger.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UListSettingDataObjectKeyRemap::InitKeyRemap(UEnhancedInputUserSettings* InputSettings,
                                                  UEnhancedPlayerMappableKeyProfile* KeyProfile, ECommonInputType DesiredInputType,
                                                  const FPlayerKeyMapping& InPlayerKeyMapping)
{
	CachedInputSettings = InputSettings;
	CachedKeyProfile = KeyProfile;
	CachedDesiredInputType = DesiredInputType;
	CachedMappingName = InPlayerKeyMapping.GetMappingName();
	CachedKeySlot = InPlayerKeyMapping.GetSlot();
}

FSlateBrush UListSettingDataObjectKeyRemap::GetSlateBrushForCurrentKey() const
{
	FSlateBrush OutSlateBrush;
	FPlayerKeyMapping* PlayerKeyMapping = GetOwningKeyMapping();
	CHECK_NULL_RETURN_VALUE(PlayerKeyMapping, OutSlateBrush);
	
	CHECK_NULL_RETURN_VALUE(CachedInputSettings, OutSlateBrush);
	UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(CachedInputSettings->GetLocalPlayer());
	CHECK_NULL_RETURN_VALUE(InputSubsystem, OutSlateBrush);

	const bool bHasFoundBrush = UCommonInputPlatformSettings::Get()->TryGetInputBrush(
		OutSlateBrush, PlayerKeyMapping->GetCurrentKey(),
		CachedDesiredInputType, InputSubsystem->GetCurrentGamepadName());
	if (!bHasFoundBrush)
	{
		PrintInLog(TEXT("Can't find icon for key: ") + 
			PlayerKeyMapping->GetCurrentKey().GetDisplayName().ToString());
	}
	return OutSlateBrush;
}

void UListSettingDataObjectKeyRemap::BindNewInputKey(const FKey& InNewKey)
{
	CHECK_NULL_RETURN(CachedInputSettings);
	
	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = CachedMappingName;
	KeyArgs.Slot = CachedKeySlot;
	KeyArgs.NewKey = InNewKey;
	
	FGameplayTagContainer Container;
	CachedInputSettings->MapPlayerKey(KeyArgs, Container);
	CachedInputSettings->SaveSettings();
	
	NotifyListDataModified(this);
}

bool UListSettingDataObjectKeyRemap::HasDefaultValue() const
{
	return GetOwningKeyMapping()->GetDefaultKey().IsValid();
}

bool UListSettingDataObjectKeyRemap::CanResetToDefaultValue() const
{
	return HasDefaultValue() && GetOwningKeyMapping()->IsCustomized();
}

bool UListSettingDataObjectKeyRemap::TryResetToDefaultValue()
{
	if (CanResetToDefaultValue())
	{
		GetOwningKeyMapping()->ResetToDefault();
		if (CachedInputSettings)
		{
			CachedInputSettings->SaveSettings();
			NotifyListDataModified(this, ESettingsListDataModifyReason::ResetToDefault);
			return true;
		}
	}
	return false;
}

FPlayerKeyMapping* UListSettingDataObjectKeyRemap::GetOwningKeyMapping() const
{
	CHECK_NULL_RETURN_VALUE(CachedKeyProfile, nullptr);
	
	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = CachedMappingName;
	KeyArgs.Slot = CachedKeySlot;
	FPlayerKeyMapping* FoundMapping = CachedKeyProfile->FindKeyMapping(KeyArgs);
	CHECK_NULL_RETURN_VALUE(FoundMapping, nullptr);
	return FoundMapping;
}
