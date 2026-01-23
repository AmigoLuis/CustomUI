// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectString.h"

void UListSettingDataObjectString::AddSettingEntry(const FString& NewSettingNameString, const FText& NewSettingNameText)
{
	AllSettingNameString.Add(NewSettingNameString);
	AllSettingNameText.Add(NewSettingNameText);
}
