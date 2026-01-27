// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Settings/ListEntry/ListEntryWidgetBase.h"
#include "ListEntryWidgetScalar.generated.h"

class UListSettingDataObjectScalar;
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
	// BindWidget
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonNumericTextBlock* SettingEntryNumericTextBlock;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UAnalogSlider* SettingEntryAnalogSlider;
	// BindWidget
	UPROPERTY(Transient)
	UListSettingDataObjectScalar* CachedDataObjectScalar;
};
