// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetActivatableBase.h"
#include "WidgetSettingsMenu.generated.h"

enum class ESettingsListDataModifyReason : uint8;
class UListSettingDataObjectBase;
class UWidgetSettingDetailsView;
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
	
	UPROPERTY(Transient)
	TArray<UListSettingDataObjectBase*> ResettableDataArray;
	bool bIsResettingData = false;
	void OnResetActionTriggeredInSettingsMenu();
	void OnBackActionTriggeredInSettingsMenu();
	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;
	void OnListDataModified(UListSettingDataObjectBase* InModifiedData, ESettingsListDataModifyReason InModifiedReason);
	//*** Bound widgets ***//
	UPROPERTY(meta=(BindWidget))
	UFrontEndTabListWidgetBase* SettingsTabList;
	UPROPERTY(meta=(BindWidget))
	UFrontEndCommonListView* SettingsListView;
	UPROPERTY(meta=(BindWidget))
	UWidgetSettingDetailsView* SettingDetailsView;
	//*** Bound widgets ***//
	
	// dont access this directly
	UPROPERTY(Transient)
	USettingDataRegistry* SettingsDataRegistry;
	USettingDataRegistry* GetOrCreateSettingsDataRegistry();
	
	UFUNCTION()
	void OnTabSelectedInSettingsMenu(FName TabID);
	void OnListViewItemHovered(UObject* InHoveredItem, bool bIsHovered);
	void OnListViewItemSelectionChanged(UObject* InSelectedItem);
};
