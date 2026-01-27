// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectScalar.h"

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
