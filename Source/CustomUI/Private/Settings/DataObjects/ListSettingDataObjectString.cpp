// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectString.h"

void UListSettingDataObjectString::AddSettingEntry(const FString& NewSettingNameString, const FText& NewSettingNameText)
{
	AllSettingNameString.Add(NewSettingNameString);
	AllSettingNameText.Add(NewSettingNameText);
}

void UListSettingDataObjectString::OnInitializeDataObject()
{
	Super::OnInitializeDataObject();
	//TODO:: 尝试设置存储的设置名为当前设置名
	if (AllSettingNameString.Num() > 0)
	{
		CurrentSettingNameString = AllSettingNameString[0];
		
	}
	if (!TrySetTextAccordingToString(CurrentSettingNameString))
	{
		CurrentSettingNameText = FText::FromString(TEXT("Invalid Setting Name"));
	}
}

bool UListSettingDataObjectString::TrySetTextAccordingToString(const FString& InSettingNameString)
{
	if (const int32 FoundStringIndex = AllSettingNameString.IndexOfByKey(InSettingNameString); 
		AllSettingNameString.IsValidIndex(FoundStringIndex))
	{
		CurrentSettingNameText = AllSettingNameText[FoundStringIndex];
		return true;
	}
	return false;
}
