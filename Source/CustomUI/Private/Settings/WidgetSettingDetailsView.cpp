// Amigo Luis all rights reserved


#include "Settings/WidgetSettingDetailsView.h"

#include "CheckAndLogAndReturn.h"
#include "CommonTextBlock.h"
#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "UILogger.h"
#include "Settings/DataObjects/ListSettingDataObjectBase.h"

void UWidgetSettingDetailsView::UpdateDetailViewInfo(UListSettingDataObjectBase* InDataObject,
                                                     const FString& InWidgetClass)
{
	CHECK_NULL_RETURN(InDataObject);
	TextBlockTitle->SetText(InDataObject->GetDataDisplayName());

	if (const TSoftObjectPtr<UTexture2D> SoftImage = InDataObject->GetSoftDescriptionImage(); 
		!SoftImage.IsNull())
	{
		LazyImageDescription->SetBrushFromLazyTexture(SoftImage);
		LazyImageDescription->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		LazyImageDescription->SetVisibility(ESlateVisibility::Collapsed);
	}
	RichTextBlockDescription->SetText(InDataObject->GetDescriptionRichText());

	RichTextBlockDisableReason->SetText(
		InDataObject->IsSettingDataEditable() ? FText::GetEmpty() : InDataObject->GetDisabledRichText());
}

void UWidgetSettingDetailsView::ClearDetailViewInfo()
{
	TextBlockTitle->SetText(FText::GetEmpty());
	LazyImageDescription->SetVisibility(ESlateVisibility::Collapsed);
	RichTextBlockDescription->SetText(FText::GetEmpty());
	RichTextBlockDisableReason->SetText(FText::GetEmpty());
}

void UWidgetSettingDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ClearDetailViewInfo();
}
