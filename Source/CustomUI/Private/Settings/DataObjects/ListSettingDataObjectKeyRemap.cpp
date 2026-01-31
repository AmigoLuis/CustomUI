// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectKeyRemap.h"

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
	return FSlateBrush();
}
