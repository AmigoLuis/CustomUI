// Amigo Luis all rights reserved


#include "Settings/ListEntry/ListEntryWidgetScalar.h"

#include "UILogger.h"
#include "Settings/DataObjects/ListSettingDataObjectScalar.h"
#include "AnalogSlider.h"
#include "CheckAndLogAndReturn.h"

void UListEntryWidgetScalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SettingEntryAnalogSlider->OnValueChanged.AddUniqueDynamic(this, &UListEntryWidgetScalar::OnSliderValueChanged);
	SettingEntryAnalogSlider->OnMouseCaptureBegin.AddUniqueDynamic(this, 
		&UListEntryWidgetScalar::NativeOnMouseCaptureEnterScalarWidget);
}
void UListEntryWidgetScalar::OnSliderValueChanged(float InValue)
{
	CHECK_NULL_RETURN(CachedDataObjectScalar);
	CachedDataObjectScalar->SetCurrentValue(InValue);
}

void UListEntryWidgetScalar::NativeOnMouseCaptureEnterScalarWidget()
{
	SelectThisEntryWidget();
}

void UListEntryWidgetScalar::OnListItemObjectSet(UListSettingDataObjectBase* InOwningListItemObject)
{
	Super::OnListItemObjectSet(InOwningListItemObject);
	CachedDataObjectScalar = Cast<UListSettingDataObjectScalar>(InOwningListItemObject);
	CHECK_NULL_RETURN(CachedDataObjectScalar);
	
	SettingEntryNumericTextBlock->SetNumericType(CachedDataObjectScalar->GetDisplayNumericType());
	SettingEntryNumericTextBlock->FormattingSpecification = CachedDataObjectScalar->GetDisplayFormattingOptions();
	SettingEntryNumericTextBlock->SetCurrentValue(CachedDataObjectScalar->GetCurrentDisplayValue());
	
	SettingEntryAnalogSlider->SetMinValue(CachedDataObjectScalar->GetDisplayValueRange().GetLowerBoundValue());
	SettingEntryAnalogSlider->SetMaxValue(CachedDataObjectScalar->GetDisplayValueRange().GetUpperBoundValue());
	SettingEntryAnalogSlider->SetStepSize(CachedDataObjectScalar->GetSliderStepSize());
	SettingEntryAnalogSlider->SetValue(CachedDataObjectScalar->GetCurrentDisplayValue());
}

void UListEntryWidgetScalar::OnOwningListItemObjectModified(UListSettingDataObjectBase* ModifiedData,
	ESettingsListDataModifyReason ModifyReason)
{
	Super::OnOwningListItemObjectModified(ModifiedData, ModifyReason);
	CHECK_NULL_RETURN(CachedDataObjectScalar);
	
	SettingEntryNumericTextBlock->SetCurrentValue(CachedDataObjectScalar->GetCurrentDisplayValue());
	SettingEntryAnalogSlider->SetValue(CachedDataObjectScalar->GetCurrentDisplayValue());
}
