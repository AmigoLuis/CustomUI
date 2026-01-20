// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "WidgetActivatableBase.generated.h"

class AFrontEndPlayerController;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UWidgetActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintPure)
	AFrontEndPlayerController* GetOwningFrontEndPlayerController();
private:
	TWeakObjectPtr<AFrontEndPlayerController> CachedOwningFrontEndPC;
};
