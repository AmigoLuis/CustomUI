// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectStringBool.h"

const FString UListSettingDataObjectStringBool::TrueString = TEXT("true");
const FString UListSettingDataObjectStringBool::FalseString = TEXT("false");
void UListSettingDataObjectStringBool::TryInitBoolValue()
{
	if (!AllSettingNameString.Contains(TrueString))
	{
		AddSettingEntry(TrueString, FText::FromString(TEXT("On")));
	}
	if (!AllSettingNameString.Contains(FalseString))
	{
		AddSettingEntry(FalseString, FText::FromString(TEXT("Off")));
	}
}

void UListSettingDataObjectStringBool::OverrideTrueDisplayText(const FText& InNewTrueDisplayText)
{
	if (!AllSettingNameString.Contains(TrueString))
	{
		AddSettingEntry(TrueString, InNewTrueDisplayText);
	}
}

void UListSettingDataObjectStringBool::OverrideFalseDisplayText(const FText& InNewFalseDisplayText)
{
	if (!AllSettingNameString.Contains(FalseString))
	{
		AddSettingEntry(FalseString, InNewFalseDisplayText);
	}
}

void UListSettingDataObjectStringBool::SetFalseAsDefaultValue()
{
	SetDefaultValueFromString(FalseString);
}

void UListSettingDataObjectStringBool::SetTrueAsDefaultValue()
{
	SetDefaultValueFromString(TrueString);
}

void UListSettingDataObjectStringBool::OnInitializeDataObject()
{
	TryInitBoolValue();
	Super::OnInitializeDataObject();
}
