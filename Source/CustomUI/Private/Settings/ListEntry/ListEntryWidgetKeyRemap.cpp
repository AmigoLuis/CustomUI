// Amigo Luis all rights reserved


#include "Settings/ListEntry/ListEntryWidgetKeyRemap.h"

#include "CheckAndLogAndReturn.h"
#include "FrontEndGameplayTags.h"
#include "UILogger.h"
#include "FunctionLibraries/UIFunctionLibrary.h"
#include "Settings/DataObjects/ListSettingDataObjectKeyRemap.h"
#include "Subsystems/UIGameInstanceSubsystem.h"
#include "Widgets/StringTableLocations.h"
#include "Widgets/WidgetKeyRemapConfirm.h"
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
	SelectThisEntryWidget();
	UUIGameInstanceSubsystem::Get(this)->
		PushWidgetSoftPtrToStackAsync(
			UUIFunctionLibrary::GetWidgetSoftFromSettings(FrontEndGameplayTags::FrontEnd_Widget_KeyRemapConfirm),
			FrontEndGameplayTags::FrontEnd_WidgetStack_Modal,
[this](EAsyncPushWidgetState PushWidgetState, 
       UWidgetActivatableBase* WidgetToPush){
	if (PushWidgetState == EAsyncPushWidgetState::CreatedAndBeforePush)
	{
		UWidgetKeyRemapConfirm* WidgetKeyRemapConfirm = Cast<UWidgetKeyRemapConfirm>(WidgetToPush);
		CHECK_NULL_RETURN(WidgetKeyRemapConfirm);
		
		WidgetKeyRemapConfirm->OnKeyDownInKeyRemapWidgetDelegate.BindUObject(this, 
			&UListEntryWidgetKeyRemap::OnKeyRemapSucceeded);
		WidgetKeyRemapConfirm->OnKeySelectCanceledInKeyRemapWidgetDelegate.BindUObject(this, 
			&UListEntryWidgetKeyRemap::OnKeyRemapCanceled);
		
		CHECK_NULL_RETURN(OwningKeyRemapObject);
		WidgetKeyRemapConfirm->SetInputTypeToListen(OwningKeyRemapObject->GetDesiredInputType());
	}
	});
}

void UListEntryWidgetKeyRemap::OnResetKeyBindingButtonClicked()
{
	LOG_ENTER_FUNCTION();
	SelectThisEntryWidget();
	CHECK_NULL_RETURN(OwningKeyRemapObject);
	
	if (!OwningKeyRemapObject->CanResetToDefaultValue())
	{
		UUIGameInstanceSubsystem::Get(this)->PushConfirmWidgetToModalStackAsync(
			EConfirmScreenType::OK, 
			GET_UN_ASSORTED_FOR_KEY("KeyBindingCanNotResetToDefaultValueTitle"),
			GET_UN_ASSORTED_FOR_KEY("KeyBindingCanNotResetToDefaultValueHint"),
[](EConfirmScreenButtonType ConfirmType){});
		return;
	}
	OwningKeyRemapObject->TryResetToDefaultValue();
}

void UListEntryWidgetKeyRemap::OnKeyRemapSucceeded(const FKey& PressedKey)
{
	CHECK_NULL_RETURN(OwningKeyRemapObject);
	
	OwningKeyRemapObject->BindNewInputKey(PressedKey);
	
	PrintInLogDisplay(TEXT("Key remapped to: " + PressedKey.GetDisplayName().ToString()));
}

void UListEntryWidgetKeyRemap::OnKeyRemapCanceled(const FString& CanceledReason)
{
	UUIGameInstanceSubsystem::Get(this)->PushConfirmWidgetToModalStackAsync(
		EConfirmScreenType::OK,
		GET_UN_ASSORTED_FOR_KEY("KeyBindingRemapTitle"),
		GET_UN_ASSORTED_FOR_KEY_DIRECT(CanceledReason),
[](EConfirmScreenButtonType ConfirmType){});
}
