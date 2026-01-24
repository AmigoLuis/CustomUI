// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetActivatableBase.h"
#include "WidgetSettingsMenu.generated.h"

class UFrontEndCommonListView;
class UFrontEndTabListWidgetBase;
class USettingDataRegistry;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UWidgetSettingsMenu : public UWidgetActivatableBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnDeactivated() override;
	virtual void NativeOnActivated() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "FrontEnd Settings Menu", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;
	FUIActionBindingHandle ResetActionHandle;
	void OnResetActionTriggeredInSettingsMenu();
	void OnBackActionTriggeredInSettingsMenu();
	//*** Bound widgets ***//
	UPROPERTY(meta=(BindWidget))
	UFrontEndTabListWidgetBase* SettingsTabList;
	UPROPERTY(meta=(BindWidget))
	UFrontEndCommonListView* SettingsListView;
	//*** Bound widgets ***//
	
	// dont access this directly
	UPROPERTY(Transient)
	USettingDataRegistry* SettingsDataRegistry;
	USettingDataRegistry* GetOrCreateSettingsDataRegistry();
	
	UFUNCTION()
	void OnTabSelectedInSettingsMenu(FName TabID);
};
