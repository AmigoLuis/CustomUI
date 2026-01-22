// Amigo Luis all rights reserved


#include "Settings/SettingDataRegistry.h"
#include "UILogger.h"
#include "Settings/DataObjects/ListSettingDataObjectCollection.h"


void USettingDataRegistry::InitSettingDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGamePlayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
}

#undef INIT_COLLECTION_TAB
#define INIT_COLLECTION_TAB(CollectionName) \
UListSettingDataObjectCollection* CollectionName##Collection = NewObject<UListSettingDataObjectCollection>();\
CollectionName##Collection->SetDataID(FName(SYMBOL_NAME_TEXT(CollectionName)TEXT("Collection")));\
CollectionName##Collection->SetDataDisplayName(FText::FromString(SYMBOL_NAME_TEXT(CollectionName)));\
RegisteredSettingsCollectionTabs.Add(CollectionName##Collection);\

void USettingDataRegistry::InitGamePlayCollectionTab()
{
	INIT_COLLECTION_TAB(Gameplay);
}

void USettingDataRegistry::InitAudioCollectionTab()
{
	INIT_COLLECTION_TAB(Audio);
}

void USettingDataRegistry::InitVideoCollectionTab()
{
	INIT_COLLECTION_TAB(Video);
}

void USettingDataRegistry::InitControlCollectionTab()
{
	INIT_COLLECTION_TAB(Control);
}
#undef INIT_COLLECTION_TAB
