#pragma once
#include "Framework/Application/IInputProcessor.h"

enum class ECommonInputType : uint8;

class FKeyRemapInputProcessor : public IInputProcessor
{
public:
	FKeyRemapInputProcessor(ECommonInputType InputType);
protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
private:
	ECommonInputType CurrentInputTypeToListen;
};
