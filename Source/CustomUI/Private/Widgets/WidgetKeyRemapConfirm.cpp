// Amigo Luis all rights reserved


#include "Widgets/WidgetKeyRemapConfirm.h"

#include "CommonInputTypeEnum.h"
#include "CommonRichTextBlock.h"
#include "UILogger.h"
#include "FunctionLibraries/KeyRemapInputProcessor.h"

void UWidgetKeyRemapConfirm::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	CachedKeyRemapInputProcessor = MakeShared<FKeyRemapInputProcessor>(CurrentInputTypeToListen);
	CHECK_NULL_RETURN(CachedKeyRemapInputProcessor);
	CachedKeyRemapInputProcessor->OnKeyDownDelegate.BindUObject(this,&UWidgetKeyRemapConfirm::OnValidKeyDown);
	CachedKeyRemapInputProcessor->OnKeySelectCanceledDelegate.BindUObject(
		this, &UWidgetKeyRemapConfirm::OnKeySelectCanceled);
	FSlateApplication::Get().RegisterInputPreProcessor(CachedKeyRemapInputProcessor, -1);
	
	FString InputDeviceName;
	switch (CurrentInputTypeToListen)
	{
	case ECommonInputType::MouseAndKeyboard:
		InputDeviceName = TEXT("Mouse and Keyboard");
		break;
	case ECommonInputType::Gamepad:
		InputDeviceName = TEXT("Gamepad");
		break;
	default:
		InputDeviceName = TEXT("Unknown Device");
		break;
	}
	const FString& DefaultHintString = TEXT("<KeyRemapDefault>Press any</> <KeyRemapHighlight>") + 
		InputDeviceName + TEXT("</> <KeyRemapDefault>key</>");
	RemapKeyDescriptionRichText->SetText(FText::FromString(DefaultHintString));
}

void UWidgetKeyRemapConfirm::OnValidKeyDown(const FKey& PressedKey)
{
}

void UWidgetKeyRemapConfirm::OnKeySelectCanceled(const FString& CanceledReason)
{
}

void UWidgetKeyRemapConfirm::SetInputTypeToListen(const ECommonInputType InInputType)
{
	CurrentInputTypeToListen = InInputType;
}

void UWidgetKeyRemapConfirm::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	CHECK_NULL_RETURN(CachedKeyRemapInputProcessor);
	FSlateApplication::Get().UnregisterInputPreProcessor(CachedKeyRemapInputProcessor);
	CachedKeyRemapInputProcessor.Reset();
}

