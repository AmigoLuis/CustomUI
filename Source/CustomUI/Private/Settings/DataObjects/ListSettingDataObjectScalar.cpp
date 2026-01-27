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

float UListSettingDataObjectScalar::GetCurrentValue() const
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

float UListSettingDataObjectScalar::StringToFloat(const FString& InValueString) const
{
	float ConvertedValue = 0.0f;
	LexFromString(ConvertedValue, *InValueString);
	return ConvertedValue;
}
