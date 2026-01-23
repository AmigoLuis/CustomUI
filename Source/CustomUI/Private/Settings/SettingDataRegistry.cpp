// Amigo Luis all rights reserved


#include "Settings/SettingDataRegistry.h"
#include "UILogger.h"
#include "Settings/DataObjects/ListSettingDataObjectCollection.h"
#include "Settings/DataObjects/ListSettingDataObjectString.h"


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

#undef INIT_CHILD_DATA_AND_SET_ID_NAME
#define INIT_CHILD_DATA_AND_SET_ID_NAME(ChileName) \
UListSettingDataObjectString* ChileName = NewObject<UListSettingDataObjectString>();\
ChileName->SetDataID(FName(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetDataDisplayName(FText::FromString(SYMBOL_NAME_TEXT(ChileName)));

#undef ADD_CHILD_TO_COLLECTION
#define ADD_CHILD_TO_COLLECTION(ChileName, CollectionName) CollectionName##Collection->AddChildData(ChileName);

#undef ADD_CHILD_SETTING_NAME
#define ADD_CHILD_SETTING_NAME(ChileName,SettingName) \
ChileName->AddSettingEntry(SYMBOL_NAME_TEXT(SettingName), FText::FromString(SYMBOL_NAME_TEXT(SettingName)));

TArray<UListSettingDataObjectBase*> USettingDataRegistry::GetListSourceItemsBySelectedTabId(const FName& InCollectionID)
{
	const auto FoundCollectionPtr = 
		RegisteredSettingsCollectionTabs.FindByPredicate([&InCollectionID](const UListSettingDataObjectBase* Item)
	{
		return InCollectionID == Item->GetDataID();
	});
	LOG_STRING_PTR(InCollectionID.ToString());
	CHECK_NULL_RETURN_VALUE(FoundCollectionPtr, TArray<UListSettingDataObjectBase*>());
	return (*FoundCollectionPtr)->GetAllChildrenDataObjects();
}

void USettingDataRegistry::InitGamePlayCollectionTab()
{
	INIT_COLLECTION_TAB(Gameplay);
	INIT_CHILD_DATA_AND_SET_ID_NAME(Difficulty);
	ADD_CHILD_SETTING_NAME(Difficulty, Easy);
	ADD_CHILD_SETTING_NAME(Difficulty, Normal);
	ADD_CHILD_SETTING_NAME(Difficulty, Hard);
	ADD_CHILD_TO_COLLECTION(Difficulty, Gameplay);
	
	INIT_CHILD_DATA_AND_SET_ID_NAME(AutoSave);
	ADD_CHILD_SETTING_NAME(AutoSave, On);
	ADD_CHILD_SETTING_NAME(AutoSave, Off);
	ADD_CHILD_TO_COLLECTION(AutoSave, Gameplay);
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
