// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectCollection.h"

#include "CheckAndLogAndReturn.h"
#include "UILogger.h"

TArray<UListSettingDataObjectBase*> UListSettingDataObjectCollection::GetAllChildrenDataObjects() const
{
	return ChildrenDataObjects;
}

bool UListSettingDataObjectCollection::HasChildrenData() const
{
	return !ChildrenDataObjects.IsEmpty();
}

void UListSettingDataObjectCollection::AddChildData(UListSettingDataObjectBase* InChildData)
{
	CHECK_NULL_RETURN(InChildData);
	InChildData->InitializeDataObject();
	InChildData->SetParentDataRaw(this);
	ChildrenDataObjects.Add(InChildData);
}
