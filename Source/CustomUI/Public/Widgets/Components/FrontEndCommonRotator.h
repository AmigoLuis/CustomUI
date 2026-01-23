// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "FrontEndCommonRotator.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class CUSTOMUI_API UFrontEndCommonRotator : public UCommonRotator
{
	GENERATED_BODY()
public:
	void SetSelectedItemByText(const FText& InTextOption);
};
