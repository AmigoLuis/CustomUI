// Amigo Luis all rights reserved

#include "Widgets/WidgetConfirmation.h"

#include "CheckAndLogAndReturn.h"
#include "CommonTextBlock.h"
#include "UILogger.h"
#include "Components/DynamicEntryBox.h"
#include "Enums/FrontEndEnumTypes.h"
#include "Widgets/Components/FrontEndButtonBase.h"
#include "ICommonInputModule.h"
#include "PrintInLog.h"
#include "Widgets/StringTableLocations.h"

UConfirmWidgetInfoObject* UConfirmWidgetInfoObject::CreateConfirmWidget(const FText& Title, const FText& Message, 
                                                                        const EConfirmScreenType ConfirmationWidgetType)
{
	UConfirmWidgetInfoObject* InfoObject = NewObject<UConfirmWidgetInfoObject>();
	InfoObject->WidgetTitle = Title;
	InfoObject->WidgetMessage = Message;

	if (ConfirmationWidgetType == EConfirmScreenType::OK)
	{
		FConfirmWidgetButtonInfo ButtonInfo;
		ButtonInfo.ButtonText = GET_MAIN_MENU_FOR_KEY_DIRECT(TEXT("OK"));
		ButtonInfo.ConfirmationChoiceType = EConfirmScreenButtonType::Closed;
		InfoObject->AvailableButtonsInfo.Add(ButtonInfo);
	}
	else if (ConfirmationWidgetType != EConfirmScreenType::Unknown)
	{
		FText YesButtonText;
		FText NoButtonText;
		FillButtonTextBasedOnEConfirmationWidgetType(
			YesButtonText,
			NoButtonText, 
			ConfirmationWidgetType);
		
		FConfirmWidgetButtonInfo YesButtonInfo;
		YesButtonInfo.ButtonText = YesButtonText;
		YesButtonInfo.ConfirmationChoiceType = EConfirmScreenButtonType::Confirmed;
		InfoObject->AvailableButtonsInfo.Add(YesButtonInfo);
		
		FConfirmWidgetButtonInfo NoButtonInfo;
		NoButtonInfo.ButtonText = NoButtonText;
		NoButtonInfo.ConfirmationChoiceType = EConfirmScreenButtonType::Canceled;
		InfoObject->AvailableButtonsInfo.Add(NoButtonInfo);
	}
	else
	{
		PrintInLog(TEXT("should not create CreateConfirmWidget for Type: ") + 
			UEnum::GetValueAsString(ConfirmationWidgetType), Error);
	}
	return InfoObject;
}

void UConfirmWidgetInfoObject::FillButtonTextBasedOnEConfirmationWidgetType(
	FText& InConfirmButtonText,
	FText& InCancelButtonText,
	const EConfirmScreenType ConfirmationWidgetType)
{
	switch (ConfirmationWidgetType)
	{
	case EConfirmScreenType::YesNo:
		InConfirmButtonText = GET_MAIN_MENU_FOR_KEY_DIRECT(TEXT("Yes"));
		InCancelButtonText = GET_MAIN_MENU_FOR_KEY_DIRECT(TEXT("No"));
		return;
	case EConfirmScreenType::OkCancel:
		InConfirmButtonText = GET_MAIN_MENU_FOR_KEY_DIRECT(TEXT("OK"));
		InCancelButtonText = GET_MAIN_MENU_FOR_KEY_DIRECT(TEXT("Cancel"));
		return;
	case EConfirmScreenType::AcceptDecline:
		InConfirmButtonText = GET_MAIN_MENU_FOR_KEY_DIRECT(TEXT("Accept"));
		InCancelButtonText = GET_MAIN_MENU_FOR_KEY_DIRECT(TEXT("Decline"));
		return;
	default:
		PrintInLog(TEXT("can not fill button text for CreateConfirmWidgetType: ") +
		           UEnum::GetValueAsString(ConfirmationWidgetType), Error);
		return;
	}
}

void UWidgetConfirmation::InitializeConfirmWidget(UConfirmWidgetInfoObject* InConfirmationInfo,
	TFunction<void(EConfirmScreenButtonType)> InClickedButtonCallback)
{
	CHECK_NULL_RETURN(InConfirmationInfo);
	CHECK_NULL_RETURN(TitleTextBlock);
	CHECK_NULL_RETURN(MessageToConfirmTextBlock);
	CHECK_NULL_RETURN(ConfirmationButtons);
	CHECK_NULL_RETURN(InClickedButtonCallback);
	ClickedButtonCallback = InClickedButtonCallback;
	ClickedConfirmationButtons = false;
	
	TitleTextBlock->SetText(InConfirmationInfo->WidgetTitle);
	MessageToConfirmTextBlock->SetText(InConfirmationInfo->WidgetMessage);
	
	if (ConfirmationButtons->GetNumEntries() != 0)
	{
		ConfirmationButtons->Reset<UFrontEndButtonBase>(
			[](const UFrontEndButtonBase& ExistingButton)
			{
				ExistingButton.OnClicked().Clear();
			});
	}
	
	CHECK_BOOL_TRUE_RETURN_WARNING(InConfirmationInfo->AvailableButtonsInfo.IsEmpty());
	
	for (const FConfirmWidgetButtonInfo& ButtonInfo : InConfirmationInfo->AvailableButtonsInfo)
	{
		UFrontEndButtonBase* NewButton = ConfirmationButtons->CreateEntry<UFrontEndButtonBase>();
		CHECK_NULL_RETURN_WARN(NewButton);
		NewButton->SetButtonText(ButtonInfo.ButtonText);
		NewButton->OnClicked().AddLambda(
			[ButtonInfo, this](){
				ClickedButtonCallback(ButtonInfo.ConfirmationChoiceType);
				ClickedConfirmationButtons = true;
				DeactivateWidget();
			});
	}
	if (ConfirmationButtons->GetNumEntries() != 0)
	{
		// 默认聚焦到no/cancel按钮
		ConfirmationButtons->GetAllEntries().Last()->SetFocus();
	}
}

void UWidgetConfirmation::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	if (ClickedButtonCallback == nullptr) return;
	// 在出栈时调用按钮关闭回调，规避因为back action关闭widget导致不能触发回调的bug
	else if (!ClickedConfirmationButtons) // 只处理back action 导致的销毁，不处理按钮导致的（按钮点击事件中已经触发回调）
	{
		ClickedButtonCallback(EConfirmScreenButtonType::Closed);
	}
}

void UWidgetConfirmation::NativeOnActivated()
{
	Super::NativeOnActivated();
	ClickedConfirmationButtons = false;
}
