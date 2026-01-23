// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Settings/ListEntry/ListEntryWidgetBase.h"
#include "ListEntryWidgetString.generated.h"

class UListSettingDataObjectString;
class UFrontEndCommonRotator;
class UFrontEndButtonBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UListEntryWidgetString : public UListEntryWidgetBase
{
	GENERATED_BODY()

protected:
	// UListEntryWidgetBase
	virtual void OnListItemObjectSet(UListSettingDataObjectBase* InOwningListItemObject) override;
	// UListEntryWidgetBase
	// UUserWidget
	virtual void NativeOnInitialized() override;
	// UUserWidget
private:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UFrontEndButtonBase* DecreaseValueButton;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UFrontEndCommonRotator* SettingEntryValueRotator;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UFrontEndButtonBase* IncreaseValueButton;
	UPROPERTY(Transient)
	UListSettingDataObjectString* OwningListItemObject;
	
	void OnDecreaseValueButtonClicked();
	void OnIncreaseValueButtonClicked();
};
