// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetActivatableBase.h"
#include "WidgetKeyRemapConfirm.generated.h"

enum class ECommonInputType : uint8;
class FKeyRemapInputProcessor;
class UCommonRichTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UWidgetKeyRemapConfirm : public UWidgetActivatableBase
{
	GENERATED_BODY()
public:
	void SetInputTypeToListen(const ECommonInputType InInputType);
protected:
	// UCommonActivatableWidget
	virtual void NativeOnDeactivated() override;
	virtual void NativeOnActivated() override;
	// UCommonActivatableWidget
private:
	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* RemapKeyDescriptionRichText;
	
	TSharedPtr<FKeyRemapInputProcessor> CachedKeyRemapInputProcessor;
	ECommonInputType CurrentInputTypeToListen;
};
