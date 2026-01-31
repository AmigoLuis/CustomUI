// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SettingDataRegistry.generated.h"

class UListSettingDataObjectBase;
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
	[[nodiscard]] FORCEINLINE const TArray<UListSettingDataObjectCollection*>& GetRegisteredSettingTabCollections() const
	{
		return RegisteredSettingsCollectionTabs;
	}
	TArray<UListSettingDataObjectBase*> GetListSourceItemsBySelectedTabId(const FName& InCollectionID); 
private:
	void InitGamePlayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer);
	UPROPERTY(Transient)
	TArray<UListSettingDataObjectCollection*> RegisteredSettingsCollectionTabs;
};
