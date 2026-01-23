// Amigo Luis all rights reserved


#include "Widgets/FrontEndCommonListView.h"

#include "UILogger.h"
#include "Editor/WidgetCompilerLog.h"
#include "Settings/DataObjects/DA_DataListEntryMapping.h"
#include "Settings/DataObjects/ListSettingDataObjectBase.h"
#include "Settings/ListEntry/ListEntryWidgetBase.h"

#if WITH_EDITOR
M_VALIDATE_COMPILED_DEFAULTS_DEFINE(UFrontEndCommonListView, DataListEntryMapping, UDA_DataListEntryMapping)
#endif

UUserWidget& UFrontEndCommonListView::OnGenerateEntryWidgetInternal(UObject* Item,
	TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
#define DEFAULT_RESULT Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable)
	
	if (IsDesignTime())	return DEFAULT_RESULT;
	const UListSettingDataObjectBase* ItemObjectBase = Cast<UListSettingDataObjectBase>(Item);
	CHECK_NULL_RETURN_VALUE(ItemObjectBase, DEFAULT_RESULT);
	
	const TSubclassOf<UListEntryWidgetBase> FoundWidgetClass =
		DataListEntryMapping->FindEntryWidgetByDataObjectClass(ItemObjectBase);
	CHECK_NULL_RETURN_VALUE(FoundWidgetClass, DEFAULT_RESULT);
	
	return GenerateTypedEntry<UListEntryWidgetBase>(FoundWidgetClass, OwnerTable);
#undef DEFAULT_RESULT
}
