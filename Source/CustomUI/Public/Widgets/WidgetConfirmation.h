// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetActivatableBase.h"
#include "WidgetConfirmation.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UWidgetConfirmation : public UWidgetActivatableBase
{
	GENERATED_BODY()
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* TitleTextBlock;
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* MessageToConfirmTextBlock;
	UPROPERTY(meta = (BindWidget))
	UDynamicEntryBox* ConfirmationButtons;
};
