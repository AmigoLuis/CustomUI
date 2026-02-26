// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserMessageBase.generated.h"

class UCommonLazyImage;
class UCommonRichTextBlock;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class CUSTOMUI_API UUserMessageBase : public UUserWidget
{
	GENERATED_BODY()
public:
	// 消息存活时间，单位为毫秒
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AutoDestroy")
	float TimeToLiveMs = 5000.0f;
	UFUNCTION(BlueprintCallable)
	void SetUserMessageTitleText(const FString& InTitleTextKey);
	UFUNCTION(BlueprintCallable)
	void SetUserMessageDetailText(const FString& InDetailTextKey);
	UFUNCTION(BlueprintCallable)
	void SetUserMessageLazyImage();
	// 移除Widget
	void DestroySelf();
protected:
	virtual void NativeConstruct() override;
private:
#pragma region widgets
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* UserMessageTitleTextBlock;
	UPROPERTY(meta=(BindWidget)) // 消息详情
	UCommonRichTextBlock* UserMessageDetailTextBlock;
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonLazyImage* UserMessageLazyImage;
#pragma endregion widgets
	FTimerHandle DestroyTimerHandle;
	UPROPERTY()
	bool bDestroyed = false;
};
