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
#define INIT_COLLECTION_TAB \
UListSettingDataObjectCollection* COLLECTION_NAME##Collection = NewObject<UListSettingDataObjectCollection>();\
COLLECTION_NAME##Collection->SetDataID(FName(SYMBOL_NAME_TEXT(COLLECTION_NAME)TEXT("Collection")));\
COLLECTION_NAME##Collection->SetDataDisplayName(FText::FromString(SYMBOL_NAME_TEXT(COLLECTION_NAME)));\
RegisteredSettingsCollectionTabs.Add(COLLECTION_NAME##Collection);\

#undef INIT_CHILD_DATA_AND_SET_ID_NAME
#define INIT_CHILD_DATA_AND_SET_ID_NAME(ChileName) \
UListSettingDataObjectString* ChileName = NewObject<UListSettingDataObjectString>();\
ChileName->SetDataID(FName(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetDataDisplayName(FText::FromString(SYMBOL_NAME_TEXT(ChileName)));

#undef ADD_CHILD_TO_COLLECTION
#define ADD_CHILD_TO_COLLECTION(ChileName) COLLECTION_NAME##Collection->AddChildData(ChileName);

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
#undef COLLECTION_NAME
#define COLLECTION_NAME Gameplay
	INIT_COLLECTION_TAB;
	INIT_CHILD_DATA_AND_SET_ID_NAME(Difficulty);
	ADD_CHILD_SETTING_NAME(Difficulty, Easy);
	ADD_CHILD_SETTING_NAME(Difficulty, Normal);
	ADD_CHILD_SETTING_NAME(Difficulty, Hard);
	ADD_CHILD_TO_COLLECTION(Difficulty);
	
	INIT_CHILD_DATA_AND_SET_ID_NAME(AutoSave);
	ADD_CHILD_SETTING_NAME(AutoSave, On);
	ADD_CHILD_SETTING_NAME(AutoSave, Off);
	ADD_CHILD_TO_COLLECTION(AutoSave);
#undef COLLECTION_NAME
}

void USettingDataRegistry::InitAudioCollectionTab()
{
#undef COLLECTION_NAME
#define COLLECTION_NAME Audio
	INIT_COLLECTION_TAB;
#undef COLLECTION_NAME
}

void USettingDataRegistry::InitVideoCollectionTab()
{
#undef COLLECTION_NAME
#define COLLECTION_NAME Video
	INIT_COLLECTION_TAB;
#undef COLLECTION_NAME
}

void USettingDataRegistry::InitControlCollectionTab()
{
#undef COLLECTION_NAME
#define COLLECTION_NAME Control
	INIT_COLLECTION_TAB;
#undef COLLECTION_NAME
}
#undef INIT_COLLECTION_TAB
