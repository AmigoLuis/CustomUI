#include "KeyRemapInputProcessor.h"

#include "CheckAndLogAndReturn.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "ICommonInputModule.h"
#include "UILogger.h"
#include "CommonUITypes.h"
#include "PrintInLog.h"

FKeyRemapInputProcessor::FKeyRemapInputProcessor(const ECommonInputType InputType, ULocalPlayer* InWeakOwningLocalPlayer) : 
	CurrentInputTypeToListen(InputType), CachedWeakOwningLocalPlayer(InWeakOwningLocalPlayer)
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

	const UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(CachedWeakOwningLocalPlayer.Get());
	CHECK_NULL_RETURN(CommonInputSubsystem);
	ECommonInputType CurrentCommonInput = CommonInputSubsystem->GetCurrentInputType();
	
	switch (CurrentInputTypeToListen)
	{
	case ECommonInputType::MouseAndKeyboard:
		if (PressedKey.IsGamepadKey() || CurrentCommonInput == ECommonInputType::Gamepad)
		{
			OnKeySelectCanceledDelegate.ExecuteIfBound(
				TEXT("Expecting MouseAndKeyboard key pressed but pressedKey type is not MouseAndKeyboard, key remap canceled."));
			return;
		}
		break;
	case ECommonInputType::Gamepad:
		// common ui 将确认事件替换为鼠标左键，当使用 手柄按键 完成确认事件时，会变成鼠标左键，这里判断是否是这种情况
		if (CurrentCommonInput == ECommonInputType::Gamepad && PressedKey == EKeys::LeftMouseButton)
		{
			// 获取手柄的 确认事件（或者说点击事件） 绑定的key
			const FCommonInputActionDataBase* CommonInputActionData = 
				ICommonInputModule::GetSettings().GetDefaultClickAction()
			.GetRow<FCommonInputActionDataBase>(TEXT(""));
			CHECK_NULL_RETURN(CommonInputActionData);
			
			// 广播修正后的key
			OnKeyDownDelegate.ExecuteIfBound(CommonInputActionData->GetDefaultGamepadInputTypeInfo().GetKey());
			return;
		}
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
