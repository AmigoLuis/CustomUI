// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Enums/FrontEndEnumTypes.h"
#include "Widgets/WidgetActivatableBase.h"
#include "WidgetConfirmation.generated.h"

enum class EConfirmScreenButtonType : uint8;
class UDynamicEntryBox;
class UCommonTextBlock;

USTRUCT(BlueprintType)
struct FConfirmWidgetButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonType ConfirmationChoiceType = EConfirmScreenButtonType::Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonText;
};

UCLASS()
class UConfirmWidgetInfoObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	FText WidgetTitle;
	UPROPERTY(Transient)
	FText WidgetMessage;
	UPROPERTY(Transient)
	TArray<FConfirmWidgetButtonInfo> AvailableButtonsInfo;

	static UConfirmWidgetInfoObject* CreateOKConfirmWidget(const FText& Title, const FText& Message,
	                                                       const EConfirmScreenType ConfirmationWidgetType);
	static void FillButtonTextBasedOnEConfirmationWidgetType(FText& InConfirmButtonText, FText& InCancelButtonText,
	                                                         const EConfirmScreenType ConfirmationWidgetType);
};

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UWidgetConfirmation : public UWidgetActivatableBase
{
	GENERATED_BODY()
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* TitleTextBlock;
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* MessageToConfirmTextBlock;
	UPROPERTY(meta = (BindWidget))
	UDynamicEntryBox* ConfirmationButtons;

public:
	void InitializeConfirmWidget(UConfirmWidgetInfoObject* InConfirmationInfo,
	                             TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback);
};
