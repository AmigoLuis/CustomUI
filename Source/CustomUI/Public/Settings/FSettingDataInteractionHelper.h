// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"

class UFrontendGameUserSettings;
/**
 * 
 */
class CUSTOMUI_API FSettingDataInteractionHelper
{
public:
	explicit FSettingDataInteractionHelper(const FString& InSetterOrGetterFuncPath);
	FString GetValueAsString() const;
	void SetValueFromString(const FString& InStringValue) const;
private:
	FCachedPropertyPath CachedDynamicFunctionPath;
	TWeakObjectPtr<UFrontendGameUserSettings> CachedWeakGameSettings;
};
