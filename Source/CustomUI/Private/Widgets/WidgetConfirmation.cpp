// Amigo Luis all rights reserved

#include "Widgets/WidgetConfirmation.h"

#include "CommonTextBlock.h"
#include "UILogger.h"
#include "Components/DynamicEntryBox.h"
#include "Enums/FrontEndEnumTypes.h"
#include "Widgets/Components/FrontEndButtonBase.h"
#include "ICommonInputModule.h"
//TODO:键盘目前还没有完全支持，很多地方不能导航，聚焦或点击
UConfirmWidgetInfoObject* UConfirmWidgetInfoObject::CreateConfirmWidget(const FText& Title, const FText& Message, 
                                                                          const EConfirmScreenType ConfirmationWidgetType)
{
	UConfirmWidgetInfoObject* InfoObject = NewObject<UConfirmWidgetInfoObject>();
	InfoObject->WidgetTitle = Title;
	InfoObject->WidgetMessage = Message;

	if (ConfirmationWidgetType == EConfirmScreenType::OK)
	{
		FConfirmWidgetButtonInfo ButtonInfo;
		ButtonInfo.ButtonText = FText::FromString(TEXT("OK"));
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
		InConfirmButtonText = FText::FromString(TEXT("Yes"));
		InCancelButtonText = FText::FromString(TEXT("No"));
		return;
	case EConfirmScreenType::OkCancel:
		InConfirmButtonText = FText::FromString(TEXT("OK"));
		InCancelButtonText = FText::FromString(TEXT("Cancel"));
		return;
	case EConfirmScreenType::AcceptDecline:
		InConfirmButtonText = FText::FromString(TEXT("Accept"));
		InCancelButtonText = FText::FromString(TEXT("Decline"));
		return;
	default:
		PrintInLog(TEXT("can not fill button text for CreateConfirmWidgetType: ") +
		           UEnum::GetValueAsString(ConfirmationWidgetType), Error);
		return;
	}
}

void UWidgetConfirmation::InitializeConfirmWidget(UConfirmWidgetInfoObject* InConfirmationInfo,
	TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback)
{
	CHECK_NULL_RETURN(InConfirmationInfo);
	CHECK_NULL_RETURN(TitleTextBlock);
	CHECK_NULL_RETURN(MessageToConfirmTextBlock);
	CHECK_NULL_RETURN(ConfirmationButtons);
	
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
	
	CHECK_BOOL_TRUE_RETURN_WARN(InConfirmationInfo->AvailableButtonsInfo.IsEmpty());
	
	for (const FConfirmWidgetButtonInfo& ButtonInfo : InConfirmationInfo->AvailableButtonsInfo)
	{
		UFrontEndButtonBase* NewButton = ConfirmationButtons->CreateEntry<UFrontEndButtonBase>();
		CHECK_NULL_RETURN_WARN(NewButton);
		NewButton->SetButtonText(ButtonInfo.ButtonText);
		NewButton->OnClicked().AddLambda(
			[ClickedButtonCallback, ButtonInfo, this](){
				ClickedButtonCallback(ButtonInfo.ConfirmationChoiceType);
				DeactivateWidget();
			});
	}
	if (ConfirmationButtons->GetNumEntries() != 0)
	{
		// 默认聚焦到no/cancel按钮
		ConfirmationButtons->GetAllEntries().Last()->SetFocus();
	}
}
