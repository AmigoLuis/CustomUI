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
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bShowClickActionBinding = true;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bShowBackActionBinding = true;
	FUIActionBindingHandle BackActionHandle;
	FUIActionBindingHandle ClickActionHandle;
	// UUserWidget
	virtual void NativeOnInitialized() override;
	// UUserWidget
	virtual void HandleDefaultClickAction();
	virtual void HandleDefaultBackAction();
private:
	TWeakObjectPtr<AFrontEndPlayerController> CachedOwningFrontEndPC;
};
