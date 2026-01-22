// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Settings/DataObjects/ListSettingDataObjectBase.h"
#include "ListSettingDataObjectCollection.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMUI_API UListSettingDataObjectCollection : public UListSettingDataObjectBase
{
	GENERATED_BODY()
public:
	// UListSettingDataObjectBase
	virtual TArray<UListSettingDataObjectBase*> GetAllChildrenDataObjects() const override;
	virtual bool HasChildrenData() const override;
	// UListSettingDataObjectBase
	void AddChildData(UListSettingDataObjectBase* InChildData);

private:
	UPROPERTY(Transient)
	TArray<UListSettingDataObjectBase*> ChildrenDataObjects;
};
