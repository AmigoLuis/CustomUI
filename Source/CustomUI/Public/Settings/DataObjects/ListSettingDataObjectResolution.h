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
	FORCEINLINE FString GetMaxSupportedResolutions() {return MaxSupportedResolutions;}
protected:
	virtual void OnInitializeDataObject() override;
private:
	FString ResolutionToStringValue(const FIntPoint& Resolution) const;
	FText ResolutionToDisplayText(const FIntPoint& Resolution) const;
	FString MaxSupportedResolutions;
};
