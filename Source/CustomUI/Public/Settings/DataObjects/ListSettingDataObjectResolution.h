// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Settings/DataObjects/ListSettingDataObjectString.h"
#include "ListSettingDataObjectResolution.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMUI_API UListSettingDataObjectResolution : public UListSettingDataObjectString
{
	GENERATED_BODY()
public:
	void InitResolutionValue();

protected:
	virtual void OnInitializeDataObject() override;
};
