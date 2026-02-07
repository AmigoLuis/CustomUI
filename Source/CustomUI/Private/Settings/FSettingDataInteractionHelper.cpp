// Amigo Luis all rights reserved


#include "Settings/FSettingDataInteractionHelper.h"

#include "CheckAndLogAndReturn.h"
#include "UILogger.h"
#include "Settings/FrontendGameUserSettings.h"

FSettingDataInteractionHelper::FSettingDataInteractionHelper(const FString& InSetterOrGetterFuncPath)
	: CachedDynamicFunctionPath(InSetterOrGetterFuncPath), CachedWeakGameSettings(UFrontendGameUserSettings::Get())
{
}

FString FSettingDataInteractionHelper::GetValueAsString() const
{
	FString OutValue;
	UFrontendGameUserSettings* CachedGameSettings = CachedWeakGameSettings.Get();
	CHECK_NULL_RETURN_VALUE(CachedGameSettings, OutValue);
	const bool bGetPropertyAsStringSucceeded = 
		PropertyPathHelpers::GetPropertyValueAsString(CachedGameSettings, CachedDynamicFunctionPath, OutValue);
	CHECK_BOOL_FALSE_RETURN_VALUE_WARNING(bGetPropertyAsStringSucceeded,OutValue)
	return OutValue;
}

void FSettingDataInteractionHelper::SetValueFromString(const FString& InStringValue) const
{
	UFrontendGameUserSettings* CachedGameSettings = CachedWeakGameSettings.Get();
	CHECK_NULL_RETURN(CachedGameSettings);
	const bool bSetPropertyFromStringSucceeded = 
		PropertyPathHelpers::SetPropertyValueFromString(CachedGameSettings, CachedDynamicFunctionPath, InStringValue);
	if (!bSetPropertyFromStringSucceeded)
	{
		PrintInLog(
			SYMBOL_NAME_TEXT(PropertyPathHelpers::SetPropertyValueFromString) 
			TEXT(" failed, ") SYMBOL_NAME_TEXT(InStringValue) TEXT(" is ") + InStringValue + IN_FUNC_AND_LINE,
			Error);
	}
}
