// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Settings/DataObjects/ListSettingDataObjectString.h"
#include "ListSettingDataObjectInteger.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMUI_API UListSettingDataObjectInteger : public UListSettingDataObjectString
{
	GENERATED_BODY()
public:
	void AddIntegerSetting(int32 Value, const FText& InDisplayText);

protected:
	virtual void OnInitializeDataObject() override;
	virtual void OnEditDependencyDataModified(UListSettingDataObjectBase* ModifiedData,
		ESettingsListDataModifyReason ModifyReason) override;
};
