// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "FrontEndCommonTextBlock.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMUI_API UFrontEndCommonTextBlock : public UCommonTextBlock
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontEnd Button", meta=(AllowPrivateAccess = "true"))
	FString TextKeyInStringTable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontEnd Button", meta=(AllowPrivateAccess = "true"))
	FName StringTableId;
protected:
	virtual void SynchronizeProperties() override;

private:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		SynchronizeProperties();
	}
#endif
};
