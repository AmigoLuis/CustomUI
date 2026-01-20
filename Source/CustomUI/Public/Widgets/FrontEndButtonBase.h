// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontEndButtonBase.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UFrontEndButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText NewButtonText);
private:
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* ButtonTextBlock;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontEnd Button", meta=(AllowPrivateAccess = "true"))
	FText ButtonDisplayText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontEnd Button", meta=(AllowPrivateAccess = "true"))
	bool bUseUpperCaseForButtonText = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontEnd Button", meta=(AllowPrivateAccess = "true"))
	FText ButtonTooltipText;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
};
