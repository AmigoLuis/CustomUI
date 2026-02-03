// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LoadingScreenInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class ULoadingScreenInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CUSTOMUI_API ILoadingScreenInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnLoadingScreenActivated();
	// virtual void OnLoadingScreenActivated_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnLoadingScreenDeactivated();
	// virtual void OnLoadingScreenDeactivated_Implementation();
};
