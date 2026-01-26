// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectString.h"

#include "UILogger.h"
#include "Settings/FSettingDataInteractionHelper.h"

void UListSettingDataObjectString::AddSettingEntry(const FString& NewSettingNameString, const FText& NewSettingNameText)
{
	AllSettingNameString.Add(NewSettingNameString);
	AllSettingNameText.Add(NewSettingNameText);
}

void UListSettingDataObjectString::ToPreviousStringAndText()
{
	if (AllSettingNameString.IsEmpty() || AllSettingNameText.IsEmpty()) return;
	const int32 CurrentIndex = AllSettingNameString.IndexOfByKey(CurrentSettingNameString);
	const int PreviousIndex = CurrentIndex - 1;
	if (AllSettingNameString.IsValidIndex(PreviousIndex))
	{
		CurrentSettingNameString = AllSettingNameString[PreviousIndex];
	}
	else
	{
		CurrentSettingNameString = AllSettingNameString.Last();
	}
	TrySetTextAccordingToString(CurrentSettingNameString);
	// Save setting value
	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentSettingNameString);
		PrintInLog(SYMBOL_NAME_TEXT(CurrentSettingNameString) 
			TEXT(" is : ") + CurrentSettingNameString, Display);
		if (DataDynamicGetter)
		{
			PrintInLog(
				TEXT("Current ") 
				SYMBOL_NAME_TEXT(DataDynamicGetter->GetValueAsString()) 
				TEXT(" is : ") + DataDynamicGetter->GetValueAsString(), Display);	
		}
		NotifyListDataModified(this);
	}
}

void UListSettingDataObjectString::ToNextStringAndText()
{
	if (AllSettingNameString.IsEmpty() || AllSettingNameText.IsEmpty()) return;
	const auto CurrentIndex = AllSettingNameString.IndexOfByKey(CurrentSettingNameString);
	const auto NextIndex = CurrentIndex + 1;
	if (AllSettingNameString.IsValidIndex(NextIndex))
	{
		CurrentSettingNameString = AllSettingNameString[NextIndex];
	}
	else
	{
		CurrentSettingNameString = AllSettingNameString[0];
	}
	TrySetTextAccordingToString(CurrentSettingNameString);
	// Save setting value
	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentSettingNameString);
		PrintInLog(SYMBOL_NAME_TEXT(CurrentSettingNameString) 
			TEXT(" is : ") + CurrentSettingNameString, Display);
		if (DataDynamicGetter)
		{
			PrintInLog(
				TEXT("Current ") 
				SYMBOL_NAME_TEXT(DataDynamicGetter->GetValueAsString()) 
				TEXT(" is : ") + DataDynamicGetter->GetValueAsString(), Display);	
		}
		NotifyListDataModified(this);
	}
}

void UListSettingDataObjectString::OnTextChanged(const FText& InNewSelectedText)
{
	const int32 FoundIndex = AllSettingNameText.IndexOfByPredicate(
		[&InNewSelectedText](const FText& SettingNameText)
	{
		return InNewSelectedText.EqualTo(SettingNameText);
	});
	if (AllSettingNameString.IsValidIndex(FoundIndex))
	{
		CurrentSettingNameText = InNewSelectedText;
		CurrentSettingNameString = AllSettingNameString[FoundIndex];
		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentSettingNameString);
			NotifyListDataModified(this);
		}
	}
}

void UListSettingDataObjectString::OnInitializeDataObject()
{
	Super::OnInitializeDataObject();
	if (AllSettingNameString.Num() > 0)
	{
		CurrentSettingNameString = AllSettingNameString[0];
	}
	//尝试设置存储的设置名为当前设置名
	if (DataDynamicGetter && !DataDynamicGetter->GetValueAsString().IsEmpty())
	{
		CurrentSettingNameString = DataDynamicGetter->GetValueAsString();
	}
	else if (HasDefaultValue()) // 从设置中读取设置名失败，设置默认值
	{
		CurrentSettingNameString = GetDefaultValueAsString();
	}
	
	if (!TrySetTextAccordingToString(CurrentSettingNameString))
	{
		CurrentSettingNameText = FText::FromString(TEXT("Invalid Setting Name"));
	}
}

bool UListSettingDataObjectString::CanResetToDefaultValue() const
{
	return HasDefaultValue() && (CurrentSettingNameString != GetDefaultValueAsString());
}

bool UListSettingDataObjectString::TryResetToDefaultValue()
{
	if (CanResetToDefaultValue())
	{
		CurrentSettingNameString = GetDefaultValueAsString();
		TrySetTextAccordingToString(CurrentSettingNameString);
		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentSettingNameString);
			NotifyListDataModified(this, ESettingsListDataModifyReason::ResetToDefault);
			return true;
		}
	}
	return false;
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
