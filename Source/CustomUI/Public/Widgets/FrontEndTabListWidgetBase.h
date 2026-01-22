// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "FrontEndTabListWidgetBase.generated.h"

class UFrontEndButtonBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UFrontEndTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontEnd TabList Settings", 
		meta=(AllowPrivateAccess="true", ClampMin = "1", ClampMax = "10"))
	int DebugEditorPreviewTabCount = 3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontEnd TabList Settings", 
		meta=(AllowPrivateAccess="true"))
	TSubclassOf<UFrontEndButtonBase> TabButtonEntryClass;

public:
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
	void RequestRegisterTab(const FName& InTabId, const FText& InTabDisplayName);
};
