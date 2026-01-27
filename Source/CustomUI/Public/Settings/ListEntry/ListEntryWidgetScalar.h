// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Settings/ListEntry/ListEntryWidgetBase.h"
#include "ListEntryWidgetScalar.generated.h"

class UCommonNumericTextBlock;
class UAnalogSlider;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UListEntryWidgetScalar : public UListEntryWidgetBase
{
	GENERATED_BODY()
	
protected:
	// UListEntryWidgetBase
	virtual void OnListItemObjectSet(UListSettingDataObjectBase* InOwningListItemObject) override;
	virtual void OnOwningListItemObjectModified(UListSettingDataObjectBase* ModifiedData, 
		ESettingsListDataModifyReason ModifyReason) override;
	// UListEntryWidgetBase
	// UUserWidget
	virtual void NativeOnInitialized() override;
	// UUserWidget
private:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UCommonNumericTextBlock* SettingEntryNumericTextBlock;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UAnalogSlider* SettingEntryAnalogSlider;
};
