// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
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
protected:
	TSharedPtr<FSettingDataInteractionHelper> DataDynamicGetter;
	TSharedPtr<FSettingDataInteractionHelper> DataDynamicSetter;

public:
	FORCEINLINE void SetDataDynamicGetter(const TSharedPtr<FSettingDataInteractionHelper>& InDataDynamicGetter)
	{
		this->DataDynamicGetter = InDataDynamicGetter;
	}
	FORCEINLINE void SetDataDynamicSetter(const TSharedPtr<FSettingDataInteractionHelper>& InDataDynamicSetter)
	{
		this->DataDynamicSetter = InDataDynamicSetter;
	}
};
