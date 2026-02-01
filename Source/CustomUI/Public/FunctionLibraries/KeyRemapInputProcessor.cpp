#include "KeyRemapInputProcessor.h"

#include "UILogger.h"

void FKeyRemapInputProcessor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor){}

bool FKeyRemapInputProcessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	PrintInLog(TEXT("Pressed key: " + InKeyEvent.GetKey().GetDisplayName().ToString()), Display);
	return true;
}

bool FKeyRemapInputProcessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	PrintInLog(TEXT("Pressed MouseButton: " + MouseEvent.GetEffectingButton().GetDisplayName().ToString()), Display);
	return true;
}
