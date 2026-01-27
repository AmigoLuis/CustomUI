// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectScalar.h"

#include "Settings/FSettingDataInteractionHelper.h"

FCommonNumberFormattingOptions UListSettingDataObjectScalar::NoDecimal()
{
	FCommonNumberFormattingOptions Options;
	Options.RoundingMode = HalfFromZero;
	Options.MaximumFractionalDigits = 0;
	return Options;
}

FCommonNumberFormattingOptions UListSettingDataObjectScalar::WithDecimal(int32 NumFracDigit)
{
	FCommonNumberFormattingOptions Options;
	Options.RoundingMode = HalfFromZero;
	Options.MaximumFractionalDigits = NumFracDigit;
	return Options;
}

float UListSettingDataObjectScalar::GetCurrentDisplayValue() const
{
	if (DataDynamicGetter)
	{
		return FMath::GetMappedRangeValueClamped(
			OutputValueRange,
			DisplayValueRange,
			StringToFloat(DataDynamicGetter->GetValueAsString()));
	}
	return 0.0f;
}

void UListSettingDataObjectScalar::SetCurrentValue(float InNewValue)
{
	if (DataDynamicSetter)
	{
		const float ClampedNewValue = FMath::GetMappedRangeValueClamped(
			DisplayValueRange,
			OutputValueRange,
			InNewValue);
		DataDynamicSetter->SetValueFromString(LexToString(ClampedNewValue));
		NotifyListDataModified(this);
	}
}

bool UListSettingDataObjectScalar::CanResetToDefaultValue() const
{
	if (HasDefaultValue() && DataDynamicGetter)
	{
		const float DefaultFloatValue = StringToFloat(GetDefaultValueAsString());
		const float CurrentFloatValue = StringToFloat(DataDynamicGetter->GetValueAsString());
		return !FMath::IsNearlyEqual(DefaultFloatValue, CurrentFloatValue, 0.01f);
	}
	return false;
}

bool UListSettingDataObjectScalar::TryResetToDefaultValue()
{
	if (CanResetToDefaultValue() && DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(GetDefaultValueAsString());
		NotifyListDataModified(this, ESettingsListDataModifyReason::ResetToDefault);
		return true;
	}
	return false;
}

float UListSettingDataObjectScalar::StringToFloat(const FString& InValueString) const
{
	float ConvertedValue = 0.0f;
	LexFromString(ConvertedValue, *InValueString);
	return ConvertedValue;
}
