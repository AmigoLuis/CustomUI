// Amigo Luis all rights reserved


#include "Widgets/Widget_PrimaryLayout.h"
#include "UILogger.h"

void UWidget_PrimaryLayout::RegisterWidget(UPARAM(meta = (Categories = "FrontEnd.WidgetStack")) FGameplayTag Tag, 
	UCommonActivatableWidgetContainerBase* WidgetContainerBase)
{
	if (IsDesignTime() || RegisteredWidgetStackMap.Contains(Tag)) return;
	RegisteredWidgetStackMap.Add(Tag, WidgetContainerBase);
}

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::TryGetWidgetStack(
	UPARAM(meta = (Categories = "FrontEnd.WidgetStack")) FGameplayTag Tag)
{
	UCommonActivatableWidgetContainerBase* FoundWidgetStack = 
		RegisteredWidgetStackMap.FindRef(Tag);
	if (FoundWidgetStack == nullptr) 
		PrintInLog(TEXT("Can not find the widget stack by the tag:") + Tag.ToString(), Error);
	return FoundWidgetStack;
}
