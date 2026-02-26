// Amigo Luis all rights reserved


#include "Settings/SettingDataRegistry.h"

#include "CheckAndLogAndReturn.h"
#include "CommonInputTypeEnum.h"
#include "FrontEndGameplayTags.h"
#include "UILogger.h"
#include "FunctionLibraries/UIFunctionLibrary.h"
#include "Settings/FrontendGameUserSettings.h"
#include "Settings/FSettingDataInteractionHelper.h"
#include "Settings/DataObjects/ListSettingDataObjectCollection.h"
#include "Settings/DataObjects/ListSettingDataObjectInteger.h"
#include "Settings/DataObjects/ListSettingDataObjectResolution.h"
#include "Settings/DataObjects/ListSettingDataObjectScalar.h"
#include "Settings/DataObjects/ListSettingDataObjectString.h"
#include "Settings/DataObjects/ListSettingDataObjectStringBool.h"
#include "Settings/DataObjects/ListSettingDataObjectStringEnum.h"
#include "Settings/DataObjects/SettingDataEditConditionDetail.h"
#include "EnhancedInputSubsystems.h"
#include "LogMacros.h"
#include "FunctionLibraries/LanguageManager.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Settings/DataObjects/ListSettingDataObjectKeyRemap.h"
#include "Widgets/StringTableLocations.h"

// TODO:ui中鼠标应该额外添加一个back action就是鼠标右键，不然用鼠标左键click但是又要用键盘的esc返回，比较割裂
// TODO:实现可扩展的消息通知widget
// TODO:在通过键盘导航时，暂时消除鼠标，防止在循环导航时，如果list过长导致触发scroll to select index。鼠标刚好在触发悬浮的范围内的话，就会触发悬浮，影响details view里面展示的内容

#define DESCRIPTION_SUFFIX TEXT("SettingDescriptionKey")
#define NAME_SUFFIX TEXT("SettingNameKey")
#define CATEGORY_SUFFIX TEXT("Category")
#define SETTING_VALUE_KEY(SettingName, ValueIndex) \
	SYMBOL_NAME_STR(SettingName) TEXT("SettingValue") INT_TO_STR(ValueIndex)
#define SETTING_DISABLED_REASON(SettingName) SYMBOL_NAME_STR(SettingName) TEXT("SettingDisabledReason")
#define VIDEO_SETTING_DESCRIPTION_KEY(LevelInt) "GeneralVideoSettingDescription" INT_TO_STR(LevelInt)

void USettingDataRegistry::InitSettingDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGamePlayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab(InOwningLocalPlayer);
}

#undef INIT_COLLECTION_TAB
#define INIT_COLLECTION_TAB(CollectionName) \
UListSettingDataObjectCollection* CollectionName = NewObject<UListSettingDataObjectCollection>();\
CollectionName->SetDataID(FName(SYMBOL_NAME_TEXT(CollectionName)));\
CollectionName->SetDataDisplayName(GET_MAIN_MENU_FOR_KEY(SYMBOL_NAME_STR(CollectionName)));\
RegisteredSettingsCollectionTabs.Add(CollectionName);

#undef INIT_CHILD_STRING_DATA_AND_SET_ID_NAME
#define INIT_CHILD_STRING_DATA_AND_SET_ID_NAME(ChileName, StringTableLocation) \
UListSettingDataObjectString* ChileName = NewObject<UListSettingDataObjectString>();\
ChileName->SetDataID(FName(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetDataDisplayName(GET_VALUE_FOR_KEY_FROM_ST(StringTableLocation, SYMBOL_NAME_STR(ChileName) NAME_SUFFIX));\
ChileName->SetDescriptionRichText(GET_VALUE_FOR_KEY_FROM_ST(StringTableLocation, SYMBOL_NAME_STR(ChileName) DESCRIPTION_SUFFIX));\
ChileName->SetbShouldApplySettingChangeImmediately(true);

#undef INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME
#define INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(ChileName, StringTableLocation) \
UListSettingDataObjectCollection* ChileName = NewObject<UListSettingDataObjectCollection>();\
ChileName->SetDataID(FName(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetDataDisplayName(GET_VALUE_FOR_KEY_FROM_ST(StringTableLocation, SYMBOL_NAME_STR(ChileName) CATEGORY_SUFFIX));

#undef INIT_CHILD_SCALAR_DATA_AND_SET_ID_NAME
#define INIT_CHILD_SCALAR_DATA_AND_SET_ID_NAME(ChileName, StringTableLocation) \
UListSettingDataObjectScalar* ChileName = NewObject<UListSettingDataObjectScalar>();\
ChileName->SetDataID(FName(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetDataDisplayName(GET_VALUE_FOR_KEY_FROM_ST(StringTableLocation, SYMBOL_NAME_STR(ChileName) NAME_SUFFIX));\
ChileName->SetDescriptionRichText(GET_VALUE_FOR_KEY_FROM_ST(StringTableLocation, SYMBOL_NAME_STR(ChileName) DESCRIPTION_SUFFIX));\
ChileName->SetbShouldApplySettingChangeImmediately(true);

#undef INIT_CHILD_ENUM_DATA_AND_SET_ID_NAME
#define INIT_CHILD_ENUM_DATA_AND_SET_ID_NAME(ChileName, StringTableLocation) \
UListSettingDataObjectStringEnum* ChileName = NewObject<UListSettingDataObjectStringEnum>();\
ChileName->SetDataID(FName(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetDataDisplayName(GET_VALUE_FOR_KEY_FROM_ST(StringTableLocation, SYMBOL_NAME_STR(ChileName) NAME_SUFFIX));\
ChileName->SetDescriptionRichText(GET_VALUE_FOR_KEY_FROM_ST(StringTableLocation, SYMBOL_NAME_STR(ChileName) DESCRIPTION_SUFFIX));\
ChileName->SetbShouldApplySettingChangeImmediately(true);

#undef INIT_CHILD_STRING_BOOL_DATA_AND_SET_ID_NAME
#define INIT_CHILD_STRING_BOOL_DATA_AND_SET_ID_NAME(ChileName, StringTableLocation) \
UListSettingDataObjectStringBool* ChileName = NewObject<UListSettingDataObjectStringBool>();\
ChileName->SetDataID(FName(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetDataDisplayName(GET_VALUE_FOR_KEY_FROM_ST(StringTableLocation, SYMBOL_NAME_STR(ChileName) NAME_SUFFIX));\
ChileName->SetDescriptionRichText(GET_VALUE_FOR_KEY_FROM_ST(StringTableLocation, SYMBOL_NAME_STR(ChileName) DESCRIPTION_SUFFIX));\
ChileName->SetbShouldApplySettingChangeImmediately(true);

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
GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings,Get##ChileName)));\
ChileName->SetDataDynamicSetter(MakeShared<FSettingDataInteractionHelper>(\
GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings,Set##ChileName)));

// 递归遍历，获取所有FoundCollectionPtr下面的子孙节点, 
// 注意这里需要按照树的顺序去添加，所以不好改成迭代写法
static void GetAllChildRecursivelyInOrder(const UListSettingDataObjectBase* ParentNode, 
	TArray<UListSettingDataObjectBase*>& AllChildItems)
{
	if (ParentNode == nullptr || !ParentNode->HasChildrenData()) return;
	for (UListSettingDataObjectBase* Child : ParentNode->GetAllChildrenDataObjects())
	{
		if (Child == nullptr) continue;
		AllChildItems.Add(Child);
		if (Child->HasChildrenData()) GetAllChildRecursivelyInOrder(Child, AllChildItems);
	}
}

TArray<UListSettingDataObjectBase*> USettingDataRegistry::GetListSourceItemsBySelectedTabId(const FName& InCollectionID)
{
	const auto FoundCollectionPtr = 
		RegisteredSettingsCollectionTabs.FindByPredicate([&InCollectionID](const UListSettingDataObjectBase* Item)
	{
		return InCollectionID == Item->GetDataID();
	});
	LOG_STRING(InCollectionID.ToString());
	CHECK_NULL_RETURN_VALUE(FoundCollectionPtr, TArray<UListSettingDataObjectBase*>());
	TArray<UListSettingDataObjectBase*> AllChildItems;
	GetAllChildRecursivelyInOrder(*FoundCollectionPtr, AllChildItems);
	return AllChildItems;
}

void USettingDataRegistry::InitGamePlayCollectionTab()
{
	INIT_COLLECTION_TAB(GameplayCollection);
	// Difficulty
	{
		INIT_CHILD_STRING_DATA_AND_SET_ID_NAME(Difficulty, ST_UN_ASSORTED);
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
		ADD_CHILD_TO_COLLECTION(Difficulty, GameplayCollection);
	}
	//AutoSave
	{
		INIT_CHILD_STRING_DATA_AND_SET_ID_NAME(AutoSave, ST_UN_ASSORTED);
		ADD_CHILD_SETTING_NAME(AutoSave, On);
		ADD_CHILD_SETTING_NAME(AutoSave, Off);
		AutoSave->SetSoftDescriptionImage(UUIFunctionLibrary::GetSoftImageByTagFromSettings(
			FrontEndGameplayTags::FrontEnd_Image_SettingsMenuDetailTest));
		AutoSave->SetDescriptionRichText(FText::FromString(
			TEXT("The image to display can be specified in the project settings."
				" It can be anything the developer assigned in there")));
		ADD_CHILD_TO_COLLECTION(AutoSave, GameplayCollection);
	}
	
	{
		INIT_CHILD_STRING_DATA_AND_SET_ID_NAME(DisplayLanguageTag, ST_UN_ASSORTED);
		for (const auto& AvailableLanguageInfo = 
			FLanguageManager::Get().GetAvailableLanguageInfo(); 
			const auto& [LanguageTag, LanguageDisplayName] : AvailableLanguageInfo)
		{
			DisplayLanguageTag->AddSettingEntry(LanguageTag, FText::FromString(LanguageDisplayName));
		}
		ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(DisplayLanguageTag);
		ADD_CHILD_TO_COLLECTION(DisplayLanguageTag, GameplayCollection);
	}
}

void USettingDataRegistry::InitAudioCollectionTab()
{
	INIT_COLLECTION_TAB(AudioCollection);
	// Volume
	{
		INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(Volume, ST_SOUND_SETTINGS);
		ADD_CHILD_TO_COLLECTION(Volume, AudioCollection);
#define ADD_VOLUME_CHILD(ChildName) \
INIT_CHILD_SCALAR_DATA_AND_SET_ID_NAME(ChildName, ST_SOUND_SETTINGS);\
ChildName->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));\
ChildName->SetOutputValueRange(TRange<float>(0.0f, 2.0f));\
ChildName->SetSliderStepSize(0.01f);\
ChildName->SetDefaultValueFromString(LexToString(1.0f));\
ChildName->SetDisplayNumericType(ECommonNumericType::Percentage);\
ChildName->SetDisplayFormattingOptions(UListSettingDataObjectScalar::NoDecimal());/* NoDecimal:50%, One Decimal:50.5%*/\
ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(ChildName);\
ADD_CHILD_TO_COLLECTION(ChildName, Volume);\
		
		// Overall Volume
		{
			ADD_VOLUME_CHILD(OverallVolume)
		}
		// Music Volume
		{
			ADD_VOLUME_CHILD(MusicVolume)
		}
		// SoundFX Volume
		{
			ADD_VOLUME_CHILD(SoundFXVolume)
		}
		// Voice Volume
		{
			ADD_VOLUME_CHILD(VoiceVolume)
		}
		// Ambient Volume
		{
			ADD_VOLUME_CHILD(AmbientVolume)
		}
	}
	// OtherSound 
	{
		INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(OtherSound, ST_SOUND_SETTINGS);
		ADD_CHILD_TO_COLLECTION(OtherSound, AudioCollection);
		// Allow Background Music
		{
			INIT_CHILD_STRING_BOOL_DATA_AND_SET_ID_NAME(AllowBackgroundMusic, ST_SOUND_SETTINGS);
			AllowBackgroundMusic->OverrideTrueDisplayText(
				GET_SOUND_SETTING_FOR_KEY(SETTING_VALUE_KEY(AllowBackgroundMusic, 1)));
			AllowBackgroundMusic->OverrideFalseDisplayText(
				GET_SOUND_SETTING_FOR_KEY(SETTING_VALUE_KEY(AllowBackgroundMusic, 2)));
			AllowBackgroundMusic->SetTrueAsDefaultValue();
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(AllowBackgroundMusic);
			ADD_CHILD_TO_COLLECTION(AllowBackgroundMusic, OtherSound);
		}
	}
}

#define DISABLED_RICH_TEXT_STYLE TEXT("Disabled")
void USettingDataRegistry::InitVideoCollectionTab()
{
	UListSettingDataObjectStringEnum* CachedFullscreenMode;
	INIT_COLLECTION_TAB(VideoCollection);
	// Display Category
	{
#define VIDEO_SETTING_NAME_DESC_AND_VALUE_GENERAL(SettingName) \
	SettingName->SetDataDisplayName(\
		GET_VIDEO_SETTING_FOR_KEY(SYMBOL_NAME_STR(SettingName) NAME_SUFFIX));\
	SettingName->SetDescriptionRichText(\
		GET_VIDEO_SETTING_FOR_KEY(SYMBOL_NAME_STR(SettingName) DESCRIPTION_SUFFIX));\
	SettingName->AddIntegerSetting(0, \
		GET_VIDEO_SETTING_FOR_KEY(VIDEO_SETTING_DESCRIPTION_KEY(0)));\
	SettingName->AddIntegerSetting(1, \
		GET_VIDEO_SETTING_FOR_KEY(VIDEO_SETTING_DESCRIPTION_KEY(1)));\
	SettingName->AddIntegerSetting(2, \
		GET_VIDEO_SETTING_FOR_KEY(VIDEO_SETTING_DESCRIPTION_KEY(2)));\
	SettingName->AddIntegerSetting(3, \
		GET_VIDEO_SETTING_FOR_KEY(VIDEO_SETTING_DESCRIPTION_KEY(3)));\
	SettingName->AddIntegerSetting(4, \
		GET_VIDEO_SETTING_FOR_KEY(VIDEO_SETTING_DESCRIPTION_KEY(4)));
		INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(Display, ST_VIDEO_SETTINGS);
		ADD_CHILD_TO_COLLECTION(Display, VideoCollection);
		
		FSettingDataEditConditionDetail PackageBuildOnly;
		PackageBuildOnly.SetEditCondition([]()
		{
			return !GIsEditor && !GIsPlayInEditorWorld;
		});
		PackageBuildOnly.SetDisabledRichReason(
			FString::Format(TEXT("<{0}>This setting can only be adjusted in a packaged build.</>"), 
				{DISABLED_RICH_TEXT_STYLE}));
		// Fullscreen Mode
		{
			// TODO:这个好像没生效，都是全屏，没看出来有区别
			INIT_CHILD_ENUM_DATA_AND_SET_ID_NAME(FullscreenMode, ST_VIDEO_SETTINGS);
			FullscreenMode->AddEnumOption(EWindowMode::Fullscreen, 
				GET_VIDEO_SETTING_FOR_KEY(SETTING_VALUE_KEY(FullscreenMode, 1)));
			FullscreenMode->AddEnumOption(EWindowMode::WindowedFullscreen, 
				GET_VIDEO_SETTING_FOR_KEY(SETTING_VALUE_KEY(FullscreenMode, 2)));
			FullscreenMode->AddEnumOption(EWindowMode::Windowed, 
				GET_VIDEO_SETTING_FOR_KEY(SETTING_VALUE_KEY(FullscreenMode, 3)));
			FullscreenMode->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);
			FullscreenMode->AddEditCondition(PackageBuildOnly);
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(FullscreenMode);
			ADD_CHILD_TO_COLLECTION(FullscreenMode, Display);
			CachedFullscreenMode = FullscreenMode;
		}
		// ScreenResolution
		{
			UListSettingDataObjectResolution* ScreenResolution = NewObject<UListSettingDataObjectResolution>();
			ScreenResolution->SetDataID(FName(SYMBOL_NAME_TEXT(ScreenResolution)));
			ScreenResolution->SetDataDisplayName(
				GET_VIDEO_SETTING_FOR_KEY(SYMBOL_NAME_STR(ScreenResolution) NAME_SUFFIX));
			ScreenResolution->SetDescriptionRichText(
				GET_VIDEO_SETTING_FOR_KEY(SYMBOL_NAME_STR(ScreenResolution) DESCRIPTION_SUFFIX));
			ScreenResolution->InitResolutionValue();
			ScreenResolution->SetbShouldApplySettingChangeImmediately(true);
			ScreenResolution->AddEditCondition(PackageBuildOnly);
			
			FSettingDataEditConditionDetail BasedOnWindowMode;
			BasedOnWindowMode.SetEditCondition([CachedFullscreenMode]()
			{
				return CachedFullscreenMode->GetCurrentValueAsEnum<EWindowMode::Type>() 
					!= EWindowMode::WindowedFullscreen;
			});
			BasedOnWindowMode.SetDisabledRichReason(
				GET_VIDEO_SETTING_FOR_KEY(SETTING_DISABLED_REASON(BasedOnWindowMode)).ToString());
			
			BasedOnWindowMode.SetDisabledForcedStringValue(ScreenResolution->GetMaxSupportedResolutions());
			ScreenResolution->AddEditCondition(BasedOnWindowMode);
			ScreenResolution->AddEditDependencyData(CachedFullscreenMode);
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(ScreenResolution);
			ADD_CHILD_TO_COLLECTION(ScreenResolution, Display);
		}
	}
	// Graphics Category
	{
		UListSettingDataObjectInteger* CachedOverallScalabilityLevel;
		INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(Graphics, ST_VIDEO_SETTINGS);
		ADD_CHILD_TO_COLLECTION(Graphics, VideoCollection);
		// Brightness
		{
			INIT_CHILD_SCALAR_DATA_AND_SET_ID_NAME(Brightness, ST_VIDEO_SETTINGS);
			Brightness->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			// default unreal brightness is 2.2, should be in the middle
			Brightness->SetOutputValueRange(TRange<float>(1.7f, 2.7f));
			// Brightness->SetSliderStepSize(0.01f);
			Brightness->SetDefaultValueFromString(LexToString(2.2f));
			Brightness->SetDisplayNumericType(ECommonNumericType::Percentage);
			Brightness->SetDisplayFormattingOptions(UListSettingDataObjectScalar::NoDecimal());// NoDecimal:50% // One Decimal:50.5%
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(Brightness);
			ADD_CHILD_TO_COLLECTION(Brightness, Graphics);
		}
		// OverallScalabilityLevel
		{	
			UListSettingDataObjectInteger* OverallScalabilityLevel = NewObject<UListSettingDataObjectInteger>();
			OverallScalabilityLevel->SetDataID(FName(TEXT("OverallScalabilityLevel")));
			VIDEO_SETTING_NAME_DESC_AND_VALUE_GENERAL(OverallScalabilityLevel);
			OverallScalabilityLevel->SetbShouldApplySettingChangeImmediately(true);
			// OverallScalabilityLevel->SetDefaultValueFromString(LexToString(1));
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(OverallScalabilityLevel);
			ADD_CHILD_TO_COLLECTION(OverallScalabilityLevel, Graphics);
			CachedOverallScalabilityLevel = OverallScalabilityLevel;
		}
		// ResolutionScaleNormalized
		{
			INIT_CHILD_SCALAR_DATA_AND_SET_ID_NAME(ResolutionScaleNormalized, ST_VIDEO_SETTINGS);
			ResolutionScaleNormalized->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			ResolutionScaleNormalized->SetOutputValueRange(TRange<float>(0.0f, 1.0f));
			ResolutionScaleNormalized->SetDisplayNumericType(ECommonNumericType::Percentage);
			// NoDecimal:50% // One Decimal:50.5%
			ResolutionScaleNormalized->SetDisplayFormattingOptions(UListSettingDataObjectScalar::NoDecimal());
			// ResolutionScaleNormalized->SetSliderStepSize(0.01f);
			// ResolutionScaleNormalized->SetDefaultValueFromString(LexToString(1.0f));
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(ResolutionScaleNormalized);
			ResolutionScaleNormalized->SetbShouldApplySettingChangeImmediately(true);
			ResolutionScaleNormalized->AddEditDependencyData(CachedOverallScalabilityLevel);
			ADD_CHILD_TO_COLLECTION(ResolutionScaleNormalized, Graphics);
		}
		// GlobalIlluminationQuality
		{	
			UListSettingDataObjectInteger* GlobalIlluminationQuality = NewObject<UListSettingDataObjectInteger>();
			GlobalIlluminationQuality->SetDataID(FName(TEXT("GlobalIlluminationQuality")));
			GlobalIlluminationQuality->SetbShouldApplySettingChangeImmediately(true);
			VIDEO_SETTING_NAME_DESC_AND_VALUE_GENERAL(GlobalIlluminationQuality);
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(GlobalIlluminationQuality);
			GlobalIlluminationQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(GlobalIlluminationQuality);
			ADD_CHILD_TO_COLLECTION(GlobalIlluminationQuality, Graphics);
		}
		// ShadowQuality
		{	
			UListSettingDataObjectInteger* ShadowQuality = NewObject<UListSettingDataObjectInteger>();
			ShadowQuality->SetDataID(FName(TEXT("ShadowQuality")));
			ShadowQuality->SetbShouldApplySettingChangeImmediately(true);
			VIDEO_SETTING_NAME_DESC_AND_VALUE_GENERAL(ShadowQuality);
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(ShadowQuality);
			ShadowQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(ShadowQuality);
			ADD_CHILD_TO_COLLECTION(ShadowQuality, Graphics);
		}
		// AntiAliasingQuality
		// TODO:发现这里所有的Graphics修改都不会触发重置按键出现，是bug吗
		{	
			UListSettingDataObjectInteger* AntiAliasingQuality = NewObject<UListSettingDataObjectInteger>();
			AntiAliasingQuality->SetDataID(FName(TEXT("AntiAliasingQuality")));
			AntiAliasingQuality->SetbShouldApplySettingChangeImmediately(true);
			VIDEO_SETTING_NAME_DESC_AND_VALUE_GENERAL(AntiAliasingQuality);
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(AntiAliasingQuality);
			AntiAliasingQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(AntiAliasingQuality);
			ADD_CHILD_TO_COLLECTION(AntiAliasingQuality, Graphics);
		}
		// ViewDistanceQuality
		{	
			UListSettingDataObjectInteger* ViewDistanceQuality = NewObject<UListSettingDataObjectInteger>();
			ViewDistanceQuality->SetDataID(FName(TEXT("ViewDistanceQuality")));
			ViewDistanceQuality->SetbShouldApplySettingChangeImmediately(true);
			ViewDistanceQuality->SetDataDisplayName(
				GET_VIDEO_SETTING_FOR_KEY(SYMBOL_NAME_STR(ViewDistanceQuality) NAME_SUFFIX));
			ViewDistanceQuality->SetDescriptionRichText(
				GET_VIDEO_SETTING_FOR_KEY(SYMBOL_NAME_STR(ViewDistanceQuality) DESCRIPTION_SUFFIX));
			ViewDistanceQuality->AddIntegerSetting(0, 
				GET_VIDEO_SETTING_FOR_KEY("ViewDistanceQualityValue0"));
			ViewDistanceQuality->AddIntegerSetting(1, 
				GET_VIDEO_SETTING_FOR_KEY("ViewDistanceQualityValue1"));
			ViewDistanceQuality->AddIntegerSetting(2, 
				GET_VIDEO_SETTING_FOR_KEY("ViewDistanceQualityValue2"));
			ViewDistanceQuality->AddIntegerSetting(3, 
				GET_VIDEO_SETTING_FOR_KEY("ViewDistanceQualityValue3"));
			ViewDistanceQuality->AddIntegerSetting(4, 
				GET_VIDEO_SETTING_FOR_KEY("ViewDistanceQualityValue4"));
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(ViewDistanceQuality);
			ViewDistanceQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(ViewDistanceQuality);
			ADD_CHILD_TO_COLLECTION(ViewDistanceQuality, Graphics);
		}
		// TextureQuality
		{	
			UListSettingDataObjectInteger* TextureQuality = NewObject<UListSettingDataObjectInteger>();
			TextureQuality->SetDataID(FName(TEXT("TextureQuality")));
			TextureQuality->SetbShouldApplySettingChangeImmediately(true);
			VIDEO_SETTING_NAME_DESC_AND_VALUE_GENERAL(TextureQuality);
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(TextureQuality);
			TextureQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(TextureQuality);
			ADD_CHILD_TO_COLLECTION(TextureQuality, Graphics);
		}
		// VisualEffectQuality
		{	
			UListSettingDataObjectInteger* VisualEffectQuality = NewObject<UListSettingDataObjectInteger>();
			VisualEffectQuality->SetDataID(FName(TEXT("VisualEffectQuality")));
			VisualEffectQuality->SetbShouldApplySettingChangeImmediately(true);
			VIDEO_SETTING_NAME_DESC_AND_VALUE_GENERAL(VisualEffectQuality);
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(VisualEffectQuality);
			VisualEffectQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(VisualEffectQuality);
			ADD_CHILD_TO_COLLECTION(VisualEffectQuality, Graphics);
		}
		// ReflectionQuality
		{	
			UListSettingDataObjectInteger* ReflectionQuality = NewObject<UListSettingDataObjectInteger>();
			ReflectionQuality->SetDataID(FName(TEXT("ReflectionQuality")));
			ReflectionQuality->SetbShouldApplySettingChangeImmediately(true);
			VIDEO_SETTING_NAME_DESC_AND_VALUE_GENERAL(ReflectionQuality);
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(ReflectionQuality);
			ReflectionQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(ReflectionQuality);
			ADD_CHILD_TO_COLLECTION(ReflectionQuality, Graphics);
		}
		// PostProcessingQuality
		{	
			UListSettingDataObjectInteger* PostProcessingQuality = NewObject<UListSettingDataObjectInteger>();
			PostProcessingQuality->SetDataID(FName(TEXT("PostProcessingQuality")));
			PostProcessingQuality->SetbShouldApplySettingChangeImmediately(true);
			VIDEO_SETTING_NAME_DESC_AND_VALUE_GENERAL(PostProcessingQuality);
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(PostProcessingQuality);
			PostProcessingQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(PostProcessingQuality);
			ADD_CHILD_TO_COLLECTION(PostProcessingQuality, Graphics);
		}
	}
	// Advanced Graphics Category
	{
		INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(AdvancedGraphics, ST_VIDEO_SETTINGS);
		ADD_CHILD_TO_COLLECTION(AdvancedGraphics, VideoCollection);
		// VerticalSync
		{
			INIT_CHILD_STRING_BOOL_DATA_AND_SET_ID_NAME(VerticalSync, ST_VIDEO_SETTINGS);
			VerticalSync->OverrideTrueDisplayText(
				GET_VIDEO_SETTING_FOR_KEY(SETTING_VALUE_KEY(VerticalSync, 1)));
			VerticalSync->OverrideFalseDisplayText(
				GET_VIDEO_SETTING_FOR_KEY(SETTING_VALUE_KEY(VerticalSync, 2)));
			
			VerticalSync->SetDataDynamicGetter(MakeShared<FSettingDataInteractionHelper>(
			GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings,IsVSyncEnabled)));
			VerticalSync->SetDataDynamicSetter(MakeShared<FSettingDataInteractionHelper>(
			GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings,SetVSyncEnabled)));
			VerticalSync->SetFalseAsDefaultValue();
			FSettingDataEditConditionDetail IsWindowModeFullScreen;
			IsWindowModeFullScreen.SetEditCondition([CachedFullscreenMode]()
			{
				return CachedFullscreenMode->GetCurrentValueAsEnum<EWindowMode::Type>() 
					== EWindowMode::Fullscreen;
			});
			IsWindowModeFullScreen.SetDisabledRichReason(
				GET_VIDEO_SETTING_FOR_KEY(SETTING_DISABLED_REASON(VerticalSync)).ToString());
			IsWindowModeFullScreen.SetDisabledForcedStringValue(UListSettingDataObjectStringBool::GetFalseString());
			VerticalSync->AddEditCondition(IsWindowModeFullScreen);
			ADD_CHILD_TO_COLLECTION(VerticalSync, AdvancedGraphics);
		}
		// FrameRateLimit
		{
			INIT_CHILD_STRING_DATA_AND_SET_ID_NAME(FrameRateLimit, ST_VIDEO_SETTINGS);
			FrameRateLimit->AddSettingEntry(LexToString(30.0f),
				GET_VIDEO_SETTING_FOR_KEY(SETTING_VALUE_KEY(FrameRateLimit, 1)));
			FrameRateLimit->AddSettingEntry(LexToString(60.0f),
				GET_VIDEO_SETTING_FOR_KEY(SETTING_VALUE_KEY(FrameRateLimit, 2)));
			FrameRateLimit->AddSettingEntry(LexToString(90.0f),
				GET_VIDEO_SETTING_FOR_KEY(SETTING_VALUE_KEY(FrameRateLimit, 3)));
			FrameRateLimit->AddSettingEntry(LexToString(120.0f),
				GET_VIDEO_SETTING_FOR_KEY(SETTING_VALUE_KEY(FrameRateLimit, 4)));
			FrameRateLimit->AddSettingEntry(LexToString(0.0f),
				GET_VIDEO_SETTING_FOR_KEY(SETTING_VALUE_KEY(FrameRateLimit, 0)));
			FrameRateLimit->SetDefaultValueFromString(LexToString(0.0f));

			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(FrameRateLimit);
			ADD_CHILD_TO_COLLECTION(FrameRateLimit, AdvancedGraphics);
		}
	}
}

void USettingDataRegistry::InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer)
{
	INIT_COLLECTION_TAB(ControlCollection);
	
	CHECK_NULL_RETURN(InOwningLocalPlayer);
	
	using UInputSys = UEnhancedInputLocalPlayerSubsystem;
	UInputSys* InputSys = InOwningLocalPlayer->GetSubsystem<UInputSys>();
	CHECK_NULL_RETURN(InputSys);
	
	UEnhancedInputUserSettings* UserSettings = InputSys->GetUserSettings();
	CHECK_NULL_RETURN(UserSettings);
	
	// Keyboard Mouse Category
	{	
		UListSettingDataObjectCollection* Keyboard_And_Mouse = NewObject<UListSettingDataObjectCollection>();
		Keyboard_And_Mouse->SetDataID(FName(TEXT("Keyboard & Mouse")));
		Keyboard_And_Mouse->SetDataDisplayName(FText::FromString(TEXT("Keyboard & Mouse")));
		ADD_CHILD_TO_COLLECTION(Keyboard_And_Mouse, ControlCollection);
		// Keyboard_Mouse Input
		{
			FPlayerMappableKeyQueryOptions KeyboardQueryOptions;
			KeyboardQueryOptions.KeyToMatch = EKeys::S;
			KeyboardQueryOptions.bMatchBasicKeyTypes = true;
			const FString& KeyboardMappingType = TEXT("Keyboard/Mouse");
			const FString& UnknownMappingType = TEXT("Unknown");
			
			for (const auto& KeyMapping : UserSettings->GetAllAvailableKeyProfiles())
			{
				const auto Profile = KeyMapping.Value;
				if (Profile == nullptr) continue;
				for (const auto& MappingRow : Profile->GetPlayerMappingRows())
				{// TODO:这里可能导致顺序不确定，两次启动时，按键绑定的顺序可能不一致
					for (const auto& RowKeyMapping : MappingRow.Value.Mappings)
					{
						const FString* MappingTypePtr = &UnknownMappingType;
						if (Profile->DoesMappingPassQueryOptions(RowKeyMapping, KeyboardQueryOptions))
						{
							MappingTypePtr = &KeyboardMappingType;
							UListSettingDataObjectKeyRemap* KeyRemapData = NewObject<UListSettingDataObjectKeyRemap>();
							KeyRemapData->SetDataID(RowKeyMapping.GetMappingName());
							KeyRemapData->SetDataDisplayName(RowKeyMapping.GetDisplayName());
							KeyRemapData->InitKeyRemap(UserSettings, Profile, 
								ECommonInputType::MouseAndKeyboard, RowKeyMapping);
							
							Keyboard_And_Mouse->AddChildData(KeyRemapData);
						}
						PrintInLog(FString::Format(
							           TEXT("{3} Mapping ID: {0}, DisplayName: {1}, BoundKey: {2}"), {
							           	RowKeyMapping.GetMappingName().ToString(),
							           	RowKeyMapping.GetDisplayName().ToString(),
							           	RowKeyMapping.GetCurrentKey().GetDisplayName().ToString(),
							           	*MappingTypePtr
							           }), Log);
					}
				}
			}
		}
	}
	
	// Gamepad Category
	{	
		UListSettingDataObjectCollection* GamepadCategory = NewObject<UListSettingDataObjectCollection>();
		GamepadCategory->SetDataID(FName(TEXT("Gamepad")));
		GamepadCategory->SetDataDisplayName(FText::FromString(TEXT("Gamepad")));
		ADD_CHILD_TO_COLLECTION(GamepadCategory, ControlCollection);
		// Gamepad Input //TODO:后续看能否区分xbox和ps4手柄
		{
			FPlayerMappableKeyQueryOptions GamepadQueryOptions;
			GamepadQueryOptions.KeyToMatch = EKeys::Gamepad_FaceButton_Bottom;
			GamepadQueryOptions.bMatchBasicKeyTypes = true;
			const FString& GamepadMappingType = TEXT("Gamepad");
			const FString& UnknownMappingType = TEXT("Unknown");
			
			for (const auto& KeyMapping : UserSettings->GetAllAvailableKeyProfiles())
			{
				const auto Profile = KeyMapping.Value;
				if (Profile == nullptr) continue;
				for (const auto& MappingRow : Profile->GetPlayerMappingRows())
				{
					for (const auto& RowKeyMapping : MappingRow.Value.Mappings)
					{
						const FString* MappingTypePtr = &UnknownMappingType;
						if (Profile->DoesMappingPassQueryOptions(RowKeyMapping, GamepadQueryOptions))
						{
							MappingTypePtr = &GamepadMappingType;
							UListSettingDataObjectKeyRemap* KeyRemapData = NewObject<UListSettingDataObjectKeyRemap>();
							KeyRemapData->SetDataID(RowKeyMapping.GetMappingName());
							KeyRemapData->SetDataDisplayName(RowKeyMapping.GetDisplayName());
							KeyRemapData->InitKeyRemap(UserSettings, Profile, 
								ECommonInputType::Gamepad, RowKeyMapping);
							
							GamepadCategory->AddChildData(KeyRemapData);
						}
						PrintInLog(FString::Format(
							           TEXT("{3} Mapping ID: {0}, DisplayName: {1}, BoundKey: {2}"), {
							           	RowKeyMapping.GetMappingName().ToString(),
							           	RowKeyMapping.GetDisplayName().ToString(),
							           	RowKeyMapping.GetCurrentKey().GetDisplayName().ToString(),
							           	*MappingTypePtr
							           }), Log);
					}
				}
			}
		}
	}
}
#undef INIT_COLLECTION_TAB
