// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Enums/FrontEndEnumTypes.h"
#include "FunctionLibraries/UtilityMacros.h"
#include "ListSettingDataObjectBase.generated.h"

struct FSettingDataEditConditionDetail;
/**
 * 
 */
UCLASS(Abstract)
class CUSTOMUI_API UListSettingDataObjectBase : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate, 
		UListSettingDataObjectBase*, ESettingsListDataModifyReason);
	FOnListDataModifiedDelegate OnListDataModifiedDelegate;
	
	GETTER_AND_SETTER_REFERENCE(FName, DataID);
	GETTER_AND_SETTER_REFERENCE(FText, DataDisplayName);
	GETTER_AND_SETTER_REFERENCE(FText, DescriptionRichText);
	GETTER_AND_SETTER_REFERENCE(FText, DisabledRichText);
	GETTER_AND_SETTER_REFERENCE(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage);
	GETTER_AND_SETTER_VALUE(UListSettingDataObjectBase*, ParentDataRaw);
	GETTER_AND_SETTER_VALUE(bool, bShouldApplySettingChangeImmediately);
	
	void InitializeDataObject();
	
	// empty in parent class, children should override it
	virtual TArray<UListSettingDataObjectBase*> GetAllChildrenDataObjects() const
	{
		return TArray<UListSettingDataObjectBase*>();
	}
	virtual bool HasChildrenData() const { return false; }
	// call in setting resigtry to add setting data edit condition
	void AddEditCondition(const FSettingDataEditConditionDetail& SettingDataEditConditionDetail);
	bool IsSettingDataEditable();
	
	// child class should override it if it supports default value
	virtual bool HasDefaultValue() const {return false;}
	virtual bool CanResetToDefaultValue() const {return false;}
	virtual bool TryResetToDefaultValue() {return false;}
protected:
	// child can override this to allow value can be set to forced string value
	virtual bool CanSetToForcedStringValue(const FString& InForcedValue) const {return false;}
	// child can override this to specify how value can be set to forced string value
	virtual void OnSetToForcedStringValue(const FString& InForcedValue) {}
	// called in InitializeDataObject, children can override it to do custom initialization
	virtual void OnInitializeDataObject();
	
	virtual void NotifyListDataModified(UListSettingDataObjectBase* ModifiedData, 
		ESettingsListDataModifyReason ModifyReason = ESettingsListDataModifyReason::DirectlyModified);
private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;
	
	UPROPERTY(Transient)
	UListSettingDataObjectBase* ParentDataRaw;
	
	bool bShouldApplySettingChangeImmediately = false;
	TArray<FSettingDataEditConditionDetail> SettingDataEditConditionDetails;
};
