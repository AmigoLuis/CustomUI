// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_DataListEntryMapping.generated.h"

class UListEntryWidgetBase;
class UListSettingDataObjectBase;
/**
 * 
 */
UCLASS()
class CUSTOMUI_API UDA_DataListEntryMapping : public UDataAsset
{
	GENERATED_BODY()
public: TSubclassOf<UListEntryWidgetBase> FindEntryWidgetByDataObjectClass(
	const UListSettingDataObjectBase* InListDataClass) const;
private:
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UListSettingDataObjectBase>, TSubclassOf<UListEntryWidgetBase>> DataObjectListEntryMapping;
};
