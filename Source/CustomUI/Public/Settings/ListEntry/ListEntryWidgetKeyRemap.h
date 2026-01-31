// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Settings/ListEntry/ListEntryWidgetBase.h"
#include "ListEntryWidgetKeyRemap.generated.h"

class UFrontEndButtonBase;
/**
 * 
 */
UCLASS()
class CUSTOMUI_API UListEntryWidgetKeyRemap : public UListEntryWidgetBase
{
	GENERATED_BODY()
public:
private:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UFrontEndButtonBase* RemapKeyButton;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UFrontEndButtonBase* ResetKeyBindingButton;
};
