// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Settings/ListEntry/ListEntryWidgetBase.h"
#include "ListEntryWidgetString.generated.h"

class UFrontEndCommonRotator;
class UFrontEndButtonBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UListEntryWidgetString : public UListEntryWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UFrontEndButtonBase* DecreaseValueButton;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UFrontEndCommonRotator* SettingEntryValueRotator;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UFrontEndButtonBase* IncreaseValueButton;
};
