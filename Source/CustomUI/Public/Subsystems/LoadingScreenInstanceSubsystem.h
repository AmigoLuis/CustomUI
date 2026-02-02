// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoadingScreenInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMUI_API ULoadingScreenInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// USubsystem
	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	// USubsystem
private:
	void PreLoadMapWithContext(const FWorldContext& WorldContext, const FString& MapName);
	void PostLoadMap(UWorld* LoadedWorld);
};
