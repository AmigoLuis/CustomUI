// Amigo Luis all rights reserved

#include "Widgets/WidgetConfirmation.h"

#include "UILogger.h"
#include "Enums/FrontEndEnumTypes.h"

UConfirmWidgetInfoObject* UConfirmWidgetInfoObject::CreateOKConfirmWidget(const FText& Title, const FText& Message, 
	const EConfirmationWidgetType ConfirmationWidgetType)
{
	UConfirmWidgetInfoObject* InfoObject = NewObject<UConfirmWidgetInfoObject>();
	InfoObject->Title = Title;
	InfoObject->Message = Message;

	if (ConfirmationWidgetType == EConfirmationWidgetType::OK)
	{
		
		FConfirmWidgetButtonInfo ButtonInfo;
		ButtonInfo.ButtonText = FText::FromString(TEXT("OK"));
		ButtonInfo.ConfirmationChoiceType = EConfirmationChoiceType::Closed;
		InfoObject->ButtonsInfo.Add(ButtonInfo);
	} else if (ConfirmationWidgetType != EConfirmationWidgetType::Unknown)
	{
		FText YesButtonText;
		FText NoButtonText;
		FillButtonTextBasedOnEConfirmationWidgetType(
			YesButtonText,
			NoButtonText, 
			ConfirmationWidgetType);
		
		FConfirmWidgetButtonInfo YesButtonInfo;
		YesButtonInfo.ButtonText = YesButtonText;
		YesButtonInfo.ConfirmationChoiceType = EConfirmationChoiceType::Confirmed;
		InfoObject->ButtonsInfo.Add(YesButtonInfo);
		
		FConfirmWidgetButtonInfo NoButtonInfo;
		NoButtonInfo.ButtonText = NoButtonText;
		NoButtonInfo.ConfirmationChoiceType = EConfirmationChoiceType::Canceled;
		InfoObject->ButtonsInfo.Add(NoButtonInfo);
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
	const EConfirmationWidgetType ConfirmationWidgetType)
{
	switch (ConfirmationWidgetType)
	{
	case EConfirmationWidgetType::YesNo:
		InConfirmButtonText = FText::FromString(TEXT("Yes"));
		InCancelButtonText = FText::FromString(TEXT("No"));
		return;
	case EConfirmationWidgetType::OkCancel:
		InConfirmButtonText = FText::FromString(TEXT("OK"));
		InCancelButtonText = FText::FromString(TEXT("Cancel"));
		return;
	case EConfirmationWidgetType::AcceptDecline:
		InConfirmButtonText = FText::FromString(TEXT("Accept"));
		InCancelButtonText = FText::FromString(TEXT("Decline"));
		return;
	default:
		PrintInLog(TEXT("can not fill button text for CreateConfirmWidgetType: ") +
		           UEnum::GetValueAsString(ConfirmationWidgetType), Error);
		return;
	}
}
