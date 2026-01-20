// Amigo Luis all rights reserved


#include "Widgets/WidgetPrimaryLayout.h"
#include "UILogger.h"

void UWidgetPrimaryLayout::RegisterWidget(UPARAM(meta = (Categories = "FrontEnd.WidgetStack")) FGameplayTag Tag, 
	UCommonActivatableWidgetContainerBase* WidgetContainerBase)
{
	if (IsDesignTime() || RegisteredWidgetStackMap.Contains(Tag)) return;
	RegisteredWidgetStackMap.Add(Tag, WidgetContainerBase);
	PrintInLog(TEXT("widget stack registered under the tag:") + Tag.ToString(), Display);
}

UCommonActivatableWidgetContainerBase* UWidgetPrimaryLayout::FindWidgetStack(
	UPARAM(meta = (Categories = "FrontEnd.WidgetStack")) FGameplayTag WidgetStackTag)
{
	UCommonActivatableWidgetContainerBase* FoundWidgetStack = 
		RegisteredWidgetStackMap.FindRef(WidgetStackTag);
	if (FoundWidgetStack == nullptr) 
		PrintInLog(TEXT("Can not find the widget stack by the tag:") + WidgetStackTag.ToString(), Error);
	return FoundWidgetStack;
}
