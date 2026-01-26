// Amigo Luis all rights reserved


#include "Settings/WidgetSettingDetailsView.h"

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
		LazyImageDescription->SetBrushFromLazyTexture(SoftImage.Get());
		LazyImageDescription->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	RichTextBlockDescription->SetText(InDataObject->GetDescriptionRichText());
#undef DYNAMIC_DETAILS_TEXT_STYLE
#define DYNAMIC_DETAILS_TEXT_STYLE TEXT("Bold")
	const FString& DynamicDetails = 
		FString::Format(TEXT("Data Object Class: <{0}>{1}</>\n\nEntry Widget Class: <{0}>{2}</>"),{
			DYNAMIC_DETAILS_TEXT_STYLE,
			InDataObject->GetClass()->GetName(), 
			InWidgetClass});
	RichTextBlockDynamicDetails->SetText(FText::FromString(DynamicDetails));
#undef DYNAMIC_DETAILS_TEXT_STYLE
	RichTextBlockDisableReason->SetText(InDataObject->GetDisabledRichText());
}

void UWidgetSettingDetailsView::ClearDetailViewInfo()
{
	TextBlockTitle->SetText(FText::GetEmpty());
	LazyImageDescription->SetVisibility(ESlateVisibility::Collapsed);
	RichTextBlockDescription->SetText(FText::GetEmpty());
	RichTextBlockDynamicDetails->SetText(FText::GetEmpty());
	RichTextBlockDisableReason->SetText(FText::GetEmpty());
}

void UWidgetSettingDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ClearDetailViewInfo();
}
