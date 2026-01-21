// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Enums/FrontEndEnumTypes.h"
#include "Widgets/WidgetActivatableBase.h"
#include "WidgetConfirmation.generated.h"

enum class EConfirmationChoiceType : uint8;
class UDynamicEntryBox;
class UCommonTextBlock;

USTRUCT(BlueprintType)
struct FConfirmWidgetButtonInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmationChoiceType ConfirmationChoiceType = EConfirmationChoiceType::Unknown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonText;
};

UCLASS()
class UConfirmWidgetInfoObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	FText Title;
	UPROPERTY(Transient)
	FText Message;
	UPROPERTY(Transient)
	TArray<FConfirmWidgetButtonInfo> ButtonsInfo;
	
	static UConfirmWidgetInfoObject* CreateOKConfirmWidget(const FText& Title, const FText& Message, 
		const EConfirmationWidgetType ConfirmationWidgetType);
	static void FillButtonTextBasedOnEConfirmationWidgetType(FText& InConfirmButtonText, FText& InCancelButtonText,
		const EConfirmationWidgetType ConfirmationWidgetType);
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
};
