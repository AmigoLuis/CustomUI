// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ListEntryWidgetBase.generated.h"

enum class ESettingsListDataModifyReason : uint8;
class UListSettingDataObjectBase;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UListEntryWidgetBase : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On List Entry Widget Hovered"))
	void BP_OnListEntryWidgetHovered(bool bIsHovered, bool bIsEntryStillSelected);
	void NativeOnListEntryWidgetHovered(bool bIsHovered);
protected:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Get Widget To Focus For Gamepad"))
	UWidget* BP_GetWidgetToFocusForGamepad() const;
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnEntryReleased() override;
	// IUserObjectListEntry
	// UUserWidget
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	// UUserWidget
	// child should override this function to handle list item data, super call is needed
	virtual void OnListItemObjectSet(UListSettingDataObjectBase* InOwningListItemObject);
	
	// child should override this function to update UI values based on list item data, super call is not needed
	virtual void OnOwningListItemObjectModified(UListSettingDataObjectBase* ModifiedData, 
		ESettingsListDataModifyReason ModifyReason);
	
	void SelectThisEntryWidget();
private:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UCommonTextBlock* SettingEntryNameTextBlock;
};
