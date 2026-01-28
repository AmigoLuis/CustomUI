// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "FunctionLibraries/UtilityMacros.h"
#include "SettingDataEditConditionDetail.generated.h"
/**
 * 
 */
USTRUCT()
struct CUSTOMUI_API FSettingDataEditConditionDetail
{
	GENERATED_BODY()
public:
	void SetEditCondition(const TFunction<bool()>& InEditCondition) {this->EditCondition = InEditCondition;}
	bool IsConditionValid() const {return EditCondition != nullptr;}
	bool IsEditable() const
	{
		if (IsConditionValid())
		{
			return EditCondition();	
		}
		return true;
	}
	GETTER_AND_SETTER_REFERENCE(FString, DisabledRichReason)
	
	bool HasForcedStringValue() const {return DisabledForcedStringValue.IsSet();}
	[[nodiscard]] FORCEINLINE FString GetDisabledForcedStringValue() const
	{
		return DisabledForcedStringValue.GetValue();
	}
	FORCEINLINE void SetDisabledForcedStringValue(const FString& InDisabledForcedStringValue)
	{
		DisabledForcedStringValue = InDisabledForcedStringValue;
	}

private:
	TFunction<bool()> EditCondition;
	FString DisabledRichReason;
	TOptional<FString> DisabledForcedStringValue;
};
