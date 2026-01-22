// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "FunctionLibraries/UtilityMacros.h"
#include "ListSettingDataObjectBase.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMUI_API UListSettingDataObjectBase : public UObject
{
	GENERATED_BODY()
public:
	GETTER_AND_SETTER_REFERENCE(FName, DataID);
	GETTER_AND_SETTER_REFERENCE(FText, DataDisplayName);
	GETTER_AND_SETTER_REFERENCE(FText, DescriptionRichText);
	GETTER_AND_SETTER_REFERENCE(FText, DisabledRichText);
	GETTER_AND_SETTER_REFERENCE(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage);
	GETTER_AND_SETTER_VALUE(UListSettingDataObjectBase*, ParentDataRaw);
	
	void InitializeDataObject();
	
	// empty in parent class, children should override it
	virtual TArray<UListSettingDataObjectBase*> GetAllChildrenDataObjects() const
	{
		return TArray<UListSettingDataObjectBase*>();
	}
	virtual bool HasChildrenData() const { return false; }
	
protected:
	// called in InitializeDataObject, children can override it to do custom initialization
	virtual void OnInitializeDataObject();
	
private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;
	
	UPROPERTY(Transient)
	UListSettingDataObjectBase* ParentDataRaw;
};
