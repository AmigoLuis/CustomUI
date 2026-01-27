// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Settings/DataObjects/ListSettingDataObjectString.h"
#include "ListSettingDataObjectStringBool.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMUI_API UListSettingDataObjectStringBool : public UListSettingDataObjectString
{
	GENERATED_BODY()
public:
	void OverrideTrueDisplayText(const FText& InNewTrueDisplayText);
	void OverrideFalseDisplayText(const FText& InNewFalseDisplayText);
	void SetFalseAsDefaultValue();
	void SetTrueAsDefaultValue();
protected:
	// UListSettingDataObjectString
	virtual void OnInitializeDataObject() override;
	// UListSettingDataObjectString
private:
	void TryInitBoolValue();

	static const FString TrueString;
	static const FString FalseString;
};
