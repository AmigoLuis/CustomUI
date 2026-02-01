// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Settings/ListEntry/ListEntryWidgetBase.h"
#include "ListEntryWidgetKeyRemap.generated.h"

class UListSettingDataObjectKeyRemap;
class UFrontEndButtonBase;
/**
 * 
 */
UCLASS()
class CUSTOMUI_API UListEntryWidgetKeyRemap : public UListEntryWidgetBase
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
	UFrontEndButtonBase* RemapKeyButton;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UFrontEndButtonBase* ResetKeyBindingButton;
	// BindWidget
	UPROPERTY(Transient)
	UListSettingDataObjectKeyRemap* OwningKeyRemapObject;
	void OnRemapKeyButtonClicked();
	void OnResetKeyBindingButtonClicked();
	
	void OnKeyRemapSucceeded(const FKey& PressedKey);
	void OnKeyRemapCanceled(const FString& CanceledReason);
};
