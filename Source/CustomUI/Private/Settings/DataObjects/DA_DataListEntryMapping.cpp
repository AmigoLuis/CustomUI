// Amigo Luis all rights reserved


#include "Settings/DataObjects/DA_DataListEntryMapping.h"

#include "UILogger.h"
#include "Settings/DataObjects/ListSettingDataObjectBase.h"

TSubclassOf<UListEntryWidgetBase> UDA_DataListEntryMapping::FindEntryWidgetByDataObjectClass(
	const UListSettingDataObjectBase* InListDataClass) const
{
#define EMPTY_RESULT TSubclassOf<UListEntryWidgetBase>()
	CHECK_NULL_RETURN_VALUE(InListDataClass, EMPTY_RESULT);
	for (UClass* DataClass = InListDataClass->GetClass(); DataClass != nullptr; DataClass = DataClass->GetSuperClass())
	{
		if (TSubclassOf<UListSettingDataObjectBase> DataSubclassOf = TSubclassOf<UListSettingDataObjectBase>(DataClass); 
			DataSubclassOf != nullptr && DataObjectListEntryMapping.Contains(DataSubclassOf))
		{
			return DataObjectListEntryMapping.FindRef(DataSubclassOf);
		}
	}
	return EMPTY_RESULT;
#undef EMPTY_RESULT
}
