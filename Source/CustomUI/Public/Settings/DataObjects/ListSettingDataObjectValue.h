// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Distributions/Distribution.h"
#include "Settings/DataObjects/ListSettingDataObjectBase.h"
#include "ListSettingDataObjectValue.generated.h"

class FSettingDataInteractionHelper;
/**
 * 
 */
UCLASS(Abstract)
class CUSTOMUI_API UListSettingDataObjectValue : public UListSettingDataObjectBase
{
	GENERATED_BODY()
public:
	FORCEINLINE void SetDataDynamicGetter(const TSharedPtr<FSettingDataInteractionHelper>& InDataDynamicGetter)
	{
		this->DataDynamicGetter = InDataDynamicGetter;
	}
	FORCEINLINE void SetDataDynamicSetter(const TSharedPtr<FSettingDataInteractionHelper>& InDataDynamicSetter)
	{
		this->DataDynamicSetter = InDataDynamicSetter;
	}
	FORCEINLINE void SetDefaultValueFromString(const FString& InDefaultValue) {	DefaultStringValue = InDefaultValue; }

	// UListSettingDataObjectBase
	virtual bool HasDefaultValue() const override { return DefaultStringValue.IsSet(); }
	// UListSettingDataObjectBase
protected:
	FORCEINLINE FString GetDefaultValueAsString() const {return DefaultStringValue.GetValue();}
	
	TSharedPtr<FSettingDataInteractionHelper> DataDynamicGetter;
	TSharedPtr<FSettingDataInteractionHelper> DataDynamicSetter;

private: 
	TOptional<FString> DefaultStringValue;
};
