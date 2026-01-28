// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UILogger.h"
#include "Settings/DataObjects/ListSettingDataObjectString.h"
#include "ListSettingDataObjectStringEnum.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMUI_API UListSettingDataObjectStringEnum : public UListSettingDataObjectString
{
	GENERATED_BODY()
public:
	template<typename EnumType>
	void AddEnumOption(EnumType InEnumOption, const FText& InDisplayText)
	{
		const UEnum* EnumPtr = StaticEnum<EnumType>();
		CHECK_NULL_RETURN(EnumPtr);
		const FString& EnumString = EnumPtr->GetNameStringByValue(InEnumOption);
		AddSettingEntry(EnumString, InDisplayText);
	}
	
	template<typename EnumType>
	EnumType GetCurrentValueAsEnum() const
	{
		const UEnum* EnumPtr = StaticEnum<EnumType>();
		CHECK_NULL_RETURN_VALUE(EnumPtr, EnumType());
		return EnumPtr->GetValueByNameString(CurrentSettingNameString);
	}
	
	template<typename EnumType>
	void SetDefaultValueFromEnumOption(EnumType InEnumOption)
	{
		const UEnum* EnumPtr = StaticEnum<EnumType>();
		CHECK_NULL_RETURN(EnumPtr);
		const FString& EnumString = EnumPtr->GetNameStringByValue(InEnumOption);
		SetDefaultValueFromString(EnumString);
	}
};
