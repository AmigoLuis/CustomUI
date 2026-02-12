// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontEndButtonBase.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UFrontEndButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	void SetButtonText(FText NewButtonText);
	UFUNCTION(BlueprintCallable)
	void SetButtonTextByKey(const FString& ButtonTextKey);
	UFUNCTION(BlueprintCallable)
	FText GetButtonText() const;
	UFUNCTION(BlueprintCallable)
	void SetButtonDisplayImageBrush(const FSlateBrush& InBrush);
private:
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* ButtonTextBlock;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UCommonLazyImage* ButtonLazyImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontEnd Button", meta=(AllowPrivateAccess = "true"))
	FString ButtonDisplayTextKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontEnd Button", meta=(AllowPrivateAccess = "true"))
	bool bUseUpperCaseForButtonText = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontEnd Button", meta=(AllowPrivateAccess = "true"))
	FString ButtonTooltipTextKey;

protected:
	void BroadcastUpdatedButtonDescription(bool bNotBroadCastEmpty);
	// 辅助函数：手动切换悬停状态
	void ForceHoverState(bool bIsHovered);
	
	virtual void NativePreConstruct() override;
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
};
