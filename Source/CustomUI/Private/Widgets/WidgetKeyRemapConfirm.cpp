// Amigo Luis all rights reserved


#include "Widgets/WidgetKeyRemapConfirm.h"

#include "CheckAndLogAndReturn.h"
#include "CommonInputTypeEnum.h"
#include "CommonRichTextBlock.h"
#include "UILogger.h"
#include "FunctionLibraries/KeyRemapInputProcessor.h"
#include "Widgets/StringTableLocations.h"

void UWidgetKeyRemapConfirm::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	CachedKeyRemapInputProcessor = MakeShared<FKeyRemapInputProcessor>(CurrentInputTypeToListen, 
		GetOwningLocalPlayer());
	CHECK_NULL_RETURN(CachedKeyRemapInputProcessor);
	CachedKeyRemapInputProcessor->OnKeyDownDelegate.BindUObject(this,&UWidgetKeyRemapConfirm::OnValidKeyDown);
	CachedKeyRemapInputProcessor->OnKeySelectCanceledDelegate.BindUObject(
		this, &UWidgetKeyRemapConfirm::OnKeySelectCanceled);
	FSlateApplication::Get().RegisterInputPreProcessor(CachedKeyRemapInputProcessor, -1);
	
	FString InputDeviceNameKey;
	switch (CurrentInputTypeToListen)
	{
	case ECommonInputType::MouseAndKeyboard:
		InputDeviceNameKey = TEXT("Mouse and Keyboard");
		break;
	case ECommonInputType::Gamepad:
		InputDeviceNameKey = TEXT("Gamepad");
		break;
	default:
		InputDeviceNameKey = TEXT("Unknown Device");
		break;
	}
	RemapKeyDescriptionRichText->SetText(FText::Format(GET_UN_ASSORTED_FOR_KEY("KeyRemapHint"), 
		GET_UN_ASSORTED_FOR_KEY_DIRECT(InputDeviceNameKey)));
}

void UWidgetKeyRemapConfirm::OnValidKeyDown(const FKey& PressedKey)
{
	RequestDeactivateWidget([PressedKey, this]()
	{
		OnKeyDownInKeyRemapWidgetDelegate.ExecuteIfBound(PressedKey);
	});
}

void UWidgetKeyRemapConfirm::OnKeySelectCanceled(const FString& CanceledReason)
{
	RequestDeactivateWidget([CanceledReason, this]()
	{
		OnKeySelectCanceledInKeyRemapWidgetDelegate.ExecuteIfBound(CanceledReason);
	});
}

void UWidgetKeyRemapConfirm::RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback)
{
	// Delay a tick to make sure input key down is captured properly
	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this, PreDeactivateCallback](float)
	{
		if (PreDeactivateCallback)
		{
			PreDeactivateCallback();
		}
		DeactivateWidget();
		return false;
	}), 0.0f);
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

