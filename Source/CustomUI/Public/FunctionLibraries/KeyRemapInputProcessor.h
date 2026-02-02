#pragma once
#include "Framework/Application/IInputProcessor.h"

enum class ECommonInputType : uint8;

class FKeyRemapInputProcessor : public IInputProcessor
{
public:
	explicit FKeyRemapInputProcessor(const ECommonInputType InputType, ULocalPlayer* InWeakOwningLocalPlayer);
	
	DECLARE_DELEGATE_OneParam(FOnKeyDownDelegateInKeyRemapInputProcessor, const FKey& /*PressedKey*/)
	DECLARE_DELEGATE_OneParam(FOnKeySelectCanceledDelegateInKeyRemapInputProcessor, const FString& /*Canceled Reason*/)
	
	FOnKeyDownDelegateInKeyRemapInputProcessor OnKeyDownDelegate;
	FOnKeySelectCanceledDelegateInKeyRemapInputProcessor OnKeySelectCanceledDelegate;
protected:
	// IInputProcessor
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	// IInputProcessor
	void ProcessPressedKey(const FKey& PressedKey);
private:
	ECommonInputType CurrentInputTypeToListen;
	TWeakObjectPtr<ULocalPlayer> CachedWeakOwningLocalPlayer;
};
