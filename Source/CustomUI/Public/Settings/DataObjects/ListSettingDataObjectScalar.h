// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Settings/DataObjects/ListSettingDataObjectValue.h"
#include "CommonNumericTextBlock.h"
#include "ListSettingDataObjectScalar.generated.h"
/**
 * 
 */
UCLASS()
class CUSTOMUI_API UListSettingDataObjectScalar : public UListSettingDataObjectValue
{
	GENERATED_BODY()
public:
	GETTER_AND_SETTER_REFERENCE(TRange<float>, DisplayValueRange);
	GETTER_AND_SETTER_REFERENCE(TRange<float>, OutputValueRange);
	GETTER_AND_SETTER_REFERENCE(FCommonNumberFormattingOptions, DisplayFormattingOptions);
	GETTER_AND_SETTER_VALUE(float, SliderStepSize);
	GETTER_AND_SETTER_VALUE(ECommonNumericType, DisplayNumericType);
	
	static FCommonNumberFormattingOptions NoDecimal();
	static FCommonNumberFormattingOptions WithDecimal(int32 NumFracDigit);
	
	float GetCurrentDisplayValue() const;
	void SetCurrentValue(float InNewValue);
private:
	TRange<float> DisplayValueRange = TRange<float>(0.0f, 1.0f);
	TRange<float> OutputValueRange = TRange<float>(0.0f, 1.0f);
	float SliderStepSize = 0.1f;
	ECommonNumericType DisplayNumericType = ECommonNumericType::Number;
	FCommonNumberFormattingOptions DisplayFormattingOptions;

public:
	virtual bool CanResetToDefaultValue() const override;
	virtual bool TryResetToDefaultValue() override;

private:
	float StringToFloat(const FString& InValueString) const;
};
