// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetSettingDetailsView.generated.h"

class UCommonTextBlock;
class UCommonRichTextBlock;
class UCommonLazyImage;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UWidgetSettingDetailsView : public UUserWidget
{
	GENERATED_BODY()
private:
	/**** BindWidget ****/
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* TextBlockTitle;
	UPROPERTY(meta=(BindWidget))
	UCommonLazyImage* LazyImageDescription;
	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* RichTextBlockDescription;
	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* RichTextBlockDynamicDetails;
	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* RichTextBlockDisableReason;
	/**** BindWidget ****/
};
