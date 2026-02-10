// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "FrontEndCommonListView.generated.h"

class UDA_DataListEntryMapping;
/**
 * 
 */
UCLASS()
class CUSTOMUI_API UFrontEndCommonListView : public UCommonListView
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category= "FrontEnd List View")
	UDA_DataListEntryMapping* DataListEntryMapping;

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass,
		const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;

public:
	void TrySelectFirstFocusableEntry();
	void TrySelectLastFocusableEntry();
	virtual bool IsItemSelectableOrNavigable(const UObject* FirstSelectedItem);
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
	
};
