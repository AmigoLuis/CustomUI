#include "KeyRemapInputProcessor.h"

#include "CommonInputTypeEnum.h"
#include "UILogger.h"

FKeyRemapInputProcessor::FKeyRemapInputProcessor(ECommonInputType InputType) : 
CurrentInputTypeToListen(InputType)
{
}

void FKeyRemapInputProcessor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor){}

bool FKeyRemapInputProcessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	ProcessPressedKey(InKeyEvent.GetKey());
	
	PrintInLog(TEXT("Pressed key: " + InKeyEvent.GetKey().GetDisplayName().ToString()), Log);
	if (const UEnum* StaticCommonInputType = StaticEnum<ECommonInputType>())
	{
		PrintInLog(TEXT("Desired Input key Type: " + 
			StaticCommonInputType->GetValueAsString(CurrentInputTypeToListen)), Log);
	}
	
	return true;
}

bool FKeyRemapInputProcessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	ProcessPressedKey(MouseEvent.GetEffectingButton());
	PrintInLog(TEXT("Pressed MouseButton: " + MouseEvent.GetEffectingButton().GetDisplayName().ToString()), Log);
	return true;
}

void FKeyRemapInputProcessor::ProcessPressedKey(const FKey& PressedKey)
{
	if (PressedKey == EKeys::Escape)
	{
		OnKeySelectCanceledDelegate.ExecuteIfBound(TEXT("Key map has been canceled by pressing Escape."));
		return;
	}
	switch (CurrentInputTypeToListen)
	{
	case ECommonInputType::MouseAndKeyboard:
		if (PressedKey.IsGamepadKey())
		{
			OnKeySelectCanceledDelegate.ExecuteIfBound(
				TEXT("Expecting MouseAndKeyboard key pressed but pressedKey type is not MouseAndKeyboard, key remap canceled."));
			return;
		}
		break;
	case ECommonInputType::Gamepad:
		if (!PressedKey.IsGamepadKey())
		{
			OnKeySelectCanceledDelegate.ExecuteIfBound(
				TEXT("Expecting Gamepad key pressed but pressedKey type is not Gamepad, key remap canceled."));
			return;
		}
		break;
	default:
		OnKeySelectCanceledDelegate.ExecuteIfBound(
			TEXT("Unsupported key pressed, key remap canceled."));
		return;
	}
	OnKeyDownDelegate.ExecuteIfBound(PressedKey);
}
