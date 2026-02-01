// Amigo Luis all rights reserved


#include "Settings/ListEntry/ListEntryWidgetKeyRemap.h"

#include "FrontEndGameplayTags.h"
#include "UILogger.h"
#include "FunctionLibraries/UIFunctionLibrary.h"
#include "Settings/DataObjects/ListSettingDataObjectKeyRemap.h"
#include "Subsystems/UIGameInstanceSubsystem.h"
#include "Widgets/Components/FrontEndButtonBase.h"

void UListEntryWidgetKeyRemap::OnListItemObjectSet(UListSettingDataObjectBase* InOwningListItemObject)
{
	Super::OnListItemObjectSet(InOwningListItemObject);
	OwningKeyRemapObject = Cast<UListSettingDataObjectKeyRemap>(InOwningListItemObject);
	CHECK_NULL_RETURN(OwningKeyRemapObject);
	RemapKeyButton->SetButtonDisplayImageBrush(OwningKeyRemapObject->GetSlateBrushForCurrentKey());
}

void UListEntryWidgetKeyRemap::OnOwningListItemObjectModified(UListSettingDataObjectBase* ModifiedData,
	ESettingsListDataModifyReason ModifyReason)
{
	Super::OnOwningListItemObjectModified(ModifiedData, ModifyReason);
	CHECK_NULL_RETURN(OwningKeyRemapObject);
	RemapKeyButton->SetButtonDisplayImageBrush(OwningKeyRemapObject->GetSlateBrushForCurrentKey());
}

void UListEntryWidgetKeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	RemapKeyButton->OnClicked().AddUObject(this, &UListEntryWidgetKeyRemap::OnRemapKeyButtonClicked);
	ResetKeyBindingButton->OnClicked().AddUObject(this, &UListEntryWidgetKeyRemap::OnResetKeyBindingButtonClicked);
}

void UListEntryWidgetKeyRemap::OnRemapKeyButtonClicked()
{
	LOG_ENTER_FUNCTION();
	UUIGameInstanceSubsystem::Get(this)->
		PushWidgetSoftPtrToStackAsync(
			UUIFunctionLibrary::GetWidgetSoftFromSettings(FrontEndGameplayTags::FrontEnd_Widget_KeyRemapConfirm),
			FrontEndGameplayTags::FrontEnd_WidgetStack_Modal,
[](EAsyncPushWidgetState PushWidgetState, 
						UWidgetActivatableBase* WidgetToPush){
	
	});
}

void UListEntryWidgetKeyRemap::OnResetKeyBindingButtonClicked()
{
	LOG_ENTER_FUNCTION();
}
