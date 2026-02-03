// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoadingScreenInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMUI_API ULoadingScreenInstanceSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	// USubsystem
	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	// USubsystem
	// FTickableGameObject
	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	// FTickableGameObject
private:
	void PreLoadMapWithContext(const FWorldContext& WorldContext, const FString& MapName);
	void PostLoadMap(UWorld* LoadedWorld);
	void TryUpdateLoadingScreen();
	bool IsPreloadScreenActive() const;
	bool ShouldShowLoadingScreen();
	bool IsLoadingScreenNeeded() const;
	
	bool bIsLoadingMap = false;
	float HoldLoadingScreenStartUpTime = -1.0f;
};
