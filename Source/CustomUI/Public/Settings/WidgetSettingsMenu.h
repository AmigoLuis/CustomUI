// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetActivatableBase.h"
#include "WidgetSettingsMenu.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UWidgetSettingsMenu : public UWidgetActivatableBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "FrontEnd Settings Menu", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;
	FUIActionBindingHandle ResetActionHandle;
	void OnResetActionTriggeredInSettingsMenu();
	void OnBackActionTriggeredInSettingsMenu();
};
