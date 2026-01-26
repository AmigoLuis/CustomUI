// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Settings/DataObjects/ListSettingDataObjectValue.h"
#include "ListSettingDataObjectString.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMUI_API UListSettingDataObjectString : public UListSettingDataObjectValue
{
	GENERATED_BODY()
public:
	void AddSettingEntry(const FString& NewSettingNameString, const FText& NewSettingNameText);

	[[nodiscard]] FORCEINLINE const FText& GetCurrentSettingNameText() const
	{
		return CurrentSettingNameText;
	}

	[[nodiscard]] FORCEINLINE const TArray<FText>& GetAllSettingNameText() const
	{
		return AllSettingNameText;
	}

	void ToPreviousStringAndText();
	void ToNextStringAndText();
	
protected:
	FString CurrentSettingNameString;
	FText CurrentSettingNameText;
	TArray<FString> AllSettingNameString;
	TArray<FText> AllSettingNameText;
	// UListSettingDataObjectBase
	virtual void OnInitializeDataObject() override;
	// UListSettingDataObjectBase
	bool TrySetTextAccordingToString(const FString& InSettingNameString);

public:
	// UListSettingDataObjectBase
	virtual bool CanResetToDefaultValue() const override;
	virtual bool TryResetToDefaultValue() override;
	// UListSettingDataObjectBase
};
