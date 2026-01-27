// Amigo Luis all rights reserved


#include "Settings/SettingDataRegistry.h"

#include "FrontEndGameplayTags.h"
#include "UILogger.h"
#include "FunctionLibraries/UIFunctionLibrary.h"
#include "Settings/FrontendGameUserSettings.h"
#include "Settings/FSettingDataInteractionHelper.h"
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
UListSettingDataObjectCollection* CollectionName = NewObject<UListSettingDataObjectCollection>();\
CollectionName->SetDataID(FName(SYMBOL_NAME_TEXT(CollectionName)TEXT("Collection")));\
CollectionName->SetDataDisplayName(FText::FromString(SYMBOL_NAME_TEXT(CollectionName)));\
RegisteredSettingsCollectionTabs.Add(CollectionName);\

#undef INIT_CHILD_STRING_DATA_AND_SET_ID_NAME
#define INIT_CHILD_STRING_DATA_AND_SET_ID_NAME(ChileName) \
UListSettingDataObjectString* ChileName = NewObject<UListSettingDataObjectString>();\
ChileName->SetDataID(FName(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetDataDisplayName(FText::FromString(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetbShouldApplySettingChangeImmediately(true);

#undef INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME
#define INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(ChileName) \
UListSettingDataObjectCollection* ChileName = NewObject<UListSettingDataObjectCollection>();\
ChileName->SetDataID(FName(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetDataDisplayName(FText::FromString(SYMBOL_NAME_TEXT(ChileName)));
// ChileName->SetbShouldApplySettingChangeImmediately(true);

#undef ADD_CHILD_TO_COLLECTION
#define ADD_CHILD_TO_COLLECTION(ChileName, CollectionName) CollectionName->AddChildData(ChileName);

#undef ADD_CHILD_SETTING_NAME
#define ADD_CHILD_SETTING_NAME(ChileName,SettingName) \
ChileName->AddSettingEntry(SYMBOL_NAME_TEXT(SettingName), FText::FromString(SYMBOL_NAME_TEXT(SettingName)));

#undef SET_CHILD_DEFAULT_VALUE_FROM_STRING
#define SET_CHILD_DEFAULT_VALUE_FROM_STRING(ChileName,DefaultValueString) \
ChileName->SetDefaultValueFromString(SYMBOL_NAME_TEXT(DefaultValueString));

#undef ADD_CHILD_DYNAMIC_GETTER_AND_SETTER
#define ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(ChileName) \
ChileName->SetDataDynamicGetter(MakeShared<FSettingDataInteractionHelper>(\
GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings,GetCurrentGame##ChileName)));\
ChileName->SetDataDynamicSetter(MakeShared<FSettingDataInteractionHelper>(\
GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings,SetCurrentGame##ChileName)));

TArray<UListSettingDataObjectBase*> USettingDataRegistry::GetListSourceItemsBySelectedTabId(const FName& InCollectionID)
{
	const auto FoundCollectionPtr = 
		RegisteredSettingsCollectionTabs.FindByPredicate([&InCollectionID](const UListSettingDataObjectBase* Item)
	{
		return InCollectionID == Item->GetDataID();
	});
	LOG_STRING_PTR(InCollectionID.ToString());
	CHECK_NULL_RETURN_VALUE(FoundCollectionPtr, TArray<UListSettingDataObjectBase*>());
	TArray<UListSettingDataObjectBase*> AllChildItems;
	TArray<UListSettingDataObjectBase*> AllParentToGetChildFrom = {*FoundCollectionPtr};
	// 递归遍历，获取所有FoundCollectionPtr下面的子孙节点
	while (!AllParentToGetChildFrom.IsEmpty())
	{
		const UListSettingDataObjectBase* Parent = AllParentToGetChildFrom.Pop();
		if (Parent == nullptr) continue;
		const TArray<UListSettingDataObjectBase*>& TempAllChildItems = Parent->GetAllChildrenDataObjects(); 
		AllChildItems.Append(TempAllChildItems);
		for (auto TempAllChildItem : TempAllChildItems)
		{
			if (TempAllChildItem != nullptr && TempAllChildItem->HasChildrenData()) 
				AllParentToGetChildFrom.Add(TempAllChildItem);
		}
	}
	return AllChildItems;
}

void USettingDataRegistry::InitGamePlayCollectionTab()
{
	INIT_COLLECTION_TAB(Gameplay);
	INIT_CHILD_STRING_DATA_AND_SET_ID_NAME(Difficulty);
	ADD_CHILD_SETTING_NAME(Difficulty, Easy);
	ADD_CHILD_SETTING_NAME(Difficulty, Normal);
	ADD_CHILD_SETTING_NAME(Difficulty, Hard);
	SET_CHILD_DEFAULT_VALUE_FROM_STRING(Difficulty, Normal);
	ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(Difficulty);
	Difficulty->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the difficulty of the game experience.\n\n"
	"<Bold>Easy:</> Focuses on the story experience. Provides the most relaxing combat.\n\n"
	"<Bold>Normal:</> Offers slightly harder combat experience\n\n"
	"<Bold>Hard:</> Offers a much more challenging combat experience\n\n"
	"<Bold>Vert Hard:</> Provides the most challenging combat experience. Not recommended for first play through.")));
	ADD_CHILD_TO_COLLECTION(Difficulty, Gameplay);
	
	INIT_CHILD_STRING_DATA_AND_SET_ID_NAME(AutoSave);
	ADD_CHILD_SETTING_NAME(AutoSave, On);
	ADD_CHILD_SETTING_NAME(AutoSave, Off);
	AutoSave->SetSoftDescriptionImage(UUIFunctionLibrary::GetSoftImageByTagFromSettings(
		FrontEndGameplayTags::FrontEnd_Image_SettingsMenuDetailTest));
	AutoSave->SetDescriptionRichText(FText::FromString(
		TEXT("The image to display can be specified in the project settings."
	   " It can be anything the developer assigned in there")));
	ADD_CHILD_TO_COLLECTION(AutoSave, Gameplay);
}

void USettingDataRegistry::InitAudioCollectionTab()
{
	INIT_COLLECTION_TAB(Audio);
	// Volume
	{
		INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(Volume);
		ADD_CHILD_TO_COLLECTION(Volume, Audio);
		// Test Item
		{
			INIT_CHILD_STRING_DATA_AND_SET_ID_NAME(TestItem);
			ADD_CHILD_TO_COLLECTION(TestItem, Volume);
		}
	}
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
