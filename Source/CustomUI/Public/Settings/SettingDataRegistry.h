// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SettingDataRegistry.generated.h"

class UListSettingDataObjectCollection;
/**
 * 
 */
UCLASS()
class CUSTOMUI_API USettingDataRegistry : public UObject
{
	GENERATED_BODY()
public:
	void InitSettingDataRegistry(ULocalPlayer* InOwningLocalPlayer);
private:
	void InitGamePlayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlCollectionTab();
	UPROPERTY(Transient)
	TArray<UListSettingDataObjectCollection*> RegisteredSettingsCollectionTabs;

public:
	[[nodiscard]] FORCEINLINE const TArray<UListSettingDataObjectCollection*>& GetRegisteredSettingTabCollections() const
	{
		return RegisteredSettingsCollectionTabs;
	}
};
