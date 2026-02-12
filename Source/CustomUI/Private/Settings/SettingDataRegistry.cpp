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

// TODO:ui中没有点击提示但是有back提示，应该实现点击提示
// TODO:ui中鼠标应该额外添加一个back action就是鼠标右键，不然用鼠标左键click但是又要用键盘的esc返回，比较割裂
// TODO:将所有显示的文本都通过StringTable管理，复用成功经验
// TODO:实现多语言，国际化（doing）
// TODO:实现可扩展的消息通知widget
// TODO:在通过键盘导航时，暂时消除鼠标，防止在循环导航时，如果list过长导致触发scroll to select index。鼠标刚好在触发悬浮的范围内的话，就会触发悬浮，影响details view里面展示的内容
// TODO:实现根据用户操作系统语言设置默认语言
// TODO:实现语言切换项设置
# define GET_DESCRIPTION_FOR_KEY(KeyString) \
LOCTABLE("/Game/StringTables/ST_SettingMenuDescription.ST_SettingMenuDescription", KeyString)

#define DESCRIPTION_SUFFIX TEXT("SettingDescriptionKey")
#define NAME_SUFFIX TEXT("SettingNameKey")
#define SETTING_VALUE_KEY(SettingName, ValueIndex) \
	SYMBOL_NAME_STR(SettingName) TEXT("SettingValue") INT_TO_STR(ValueIndex)
#define SETTING_DISABLED_REASON(SettingName) SYMBOL_NAME_STR(SettingName) TEXT("SettingDisabledReason") 

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
CollectionName->SetDataID(FName(SYMBOL_NAME_TEXT(CollectionName)TEXT("Collection")));\
CollectionName->SetDataDisplayName(FText::FromString(SYMBOL_NAME_TEXT(CollectionName)));\
RegisteredSettingsCollectionTabs.Add(CollectionName);

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
	INIT_COLLECTION_TAB(Gameplay);
	// Difficulty
	{
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
	}
	//AutoSave
	{
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
	
	{
		INIT_CHILD_STRING_DATA_AND_SET_ID_NAME(DisplayLanguageTag);
		for (const auto& AvailableLanguageInfo = 
			FLanguageManager::Get().GetAvailableLanguageInfo(); 
			const auto& [LanguageTag, LanguageDisplayName] : AvailableLanguageInfo)
		{
			DisplayLanguageTag->AddSettingEntry(LanguageTag, FText::FromString(LanguageDisplayName));
		}
		ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(DisplayLanguageTag);
		ADD_CHILD_TO_COLLECTION(DisplayLanguageTag, Gameplay);
	}
}

void USettingDataRegistry::InitAudioCollectionTab()
{
	INIT_COLLECTION_TAB(Audio);
	// Volume
	{
		INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(Volume);
		ADD_CHILD_TO_COLLECTION(Volume, Audio);
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
	// Sound 
	{
		INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(Sound);
		ADD_CHILD_TO_COLLECTION(Sound, Audio);
		// Allow Background Music
		{
			INIT_CHILD_STRING_BOOL_DATA_AND_SET_ID_NAME(AllowBackgroundMusic, ST_SOUND_SETTINGS);
			AllowBackgroundMusic->OverrideTrueDisplayText(
				GET_SOUND_SETTING_FOR_KEY(SETTING_VALUE_KEY(AllowBackgroundMusic, 1)));
			AllowBackgroundMusic->OverrideFalseDisplayText(
				GET_SOUND_SETTING_FOR_KEY(SETTING_VALUE_KEY(AllowBackgroundMusic, 2)));
			AllowBackgroundMusic->SetTrueAsDefaultValue();
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(AllowBackgroundMusic);
			ADD_CHILD_TO_COLLECTION(AllowBackgroundMusic, Sound);
		}
	}
}

#define DISABLED_RICH_TEXT_STYLE TEXT("Disabled")
void USettingDataRegistry::InitVideoCollectionTab()
{
	UListSettingDataObjectStringEnum* CachedFullscreenMode;
	INIT_COLLECTION_TAB(Video);
	// Display Category
	{
		INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(Display);
		ADD_CHILD_TO_COLLECTION(Display, Video);
		
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
			ScreenResolution->SetDataDisplayName(FText::FromString(SYMBOL_NAME_TEXT(ScreenResolution)));
			ScreenResolution->SetDescriptionRichText(GET_DESCRIPTION_FOR_KEY("ScreenResolutionsDescKey"));
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
				FString::Format(TEXT("\n\n<{0}>The screen resolution is not editable "
						 "when fullscreen mode is borderless full screen, "
						 "the screen resolution must match maximum supported resolution.</>"), 
					{DISABLED_RICH_TEXT_STYLE}));
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
		INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(Graphics);
		ADD_CHILD_TO_COLLECTION(Graphics, Video);
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
			OverallScalabilityLevel->SetDataDisplayName(FText::FromString(TEXT("Overall Video Quality")));
			OverallScalabilityLevel->SetDescriptionRichText(GET_DESCRIPTION_FOR_KEY("OverallQualityDescKey"));
			OverallScalabilityLevel->AddIntegerSetting(0, FText::FromString(TEXT("Low")));
			OverallScalabilityLevel->AddIntegerSetting(1, FText::FromString(TEXT("Medium")));
			OverallScalabilityLevel->AddIntegerSetting(2, FText::FromString(TEXT("High")));
			OverallScalabilityLevel->AddIntegerSetting(3, FText::FromString(TEXT("Epic")));
			OverallScalabilityLevel->AddIntegerSetting(4, FText::FromString(TEXT("Cinematic")));
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
			GlobalIlluminationQuality->SetDataDisplayName(FText::FromString(TEXT("Global Illumination Quality")));
			GlobalIlluminationQuality->SetDescriptionRichText(GET_DESCRIPTION_FOR_KEY("GlobalIlluminationQualityDescKey"));
			GlobalIlluminationQuality->SetbShouldApplySettingChangeImmediately(true);
			GlobalIlluminationQuality->AddIntegerSetting(0, FText::FromString(TEXT("Low")));
			GlobalIlluminationQuality->AddIntegerSetting(1, FText::FromString(TEXT("Medium")));
			GlobalIlluminationQuality->AddIntegerSetting(2, FText::FromString(TEXT("High")));
			GlobalIlluminationQuality->AddIntegerSetting(3, FText::FromString(TEXT("Epic")));
			GlobalIlluminationQuality->AddIntegerSetting(4, FText::FromString(TEXT("Cinematic")));
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(GlobalIlluminationQuality);
			GlobalIlluminationQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(GlobalIlluminationQuality);
			ADD_CHILD_TO_COLLECTION(GlobalIlluminationQuality, Graphics);
		}
		// ShadowQuality
		{	
			UListSettingDataObjectInteger* ShadowQuality = NewObject<UListSettingDataObjectInteger>();
			ShadowQuality->SetDataID(FName(TEXT("ShadowQuality")));
			ShadowQuality->SetDataDisplayName(FText::FromString(TEXT("Shadow Quality")));
			ShadowQuality->SetDescriptionRichText(GET_DESCRIPTION_FOR_KEY("ShadowQualityDescKey"));
			ShadowQuality->SetbShouldApplySettingChangeImmediately(true);
			ShadowQuality->AddIntegerSetting(0, FText::FromString(TEXT("Low")));
			ShadowQuality->AddIntegerSetting(1, FText::FromString(TEXT("Medium")));
			ShadowQuality->AddIntegerSetting(2, FText::FromString(TEXT("High")));
			ShadowQuality->AddIntegerSetting(3, FText::FromString(TEXT("Epic")));
			ShadowQuality->AddIntegerSetting(4, FText::FromString(TEXT("Cinematic")));
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
			AntiAliasingQuality->SetDataDisplayName(FText::FromString(TEXT("AntiAliasing Quality")));
			AntiAliasingQuality->SetDescriptionRichText(GET_DESCRIPTION_FOR_KEY("AntiAliasingDescKey"));
			AntiAliasingQuality->SetbShouldApplySettingChangeImmediately(true);
			AntiAliasingQuality->AddIntegerSetting(0, FText::FromString(TEXT("Low")));
			AntiAliasingQuality->AddIntegerSetting(1, FText::FromString(TEXT("Medium")));
			AntiAliasingQuality->AddIntegerSetting(2, FText::FromString(TEXT("High")));
			AntiAliasingQuality->AddIntegerSetting(3, FText::FromString(TEXT("Epic")));
			AntiAliasingQuality->AddIntegerSetting(4, FText::FromString(TEXT("Cinematic")));
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(AntiAliasingQuality);
			AntiAliasingQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(AntiAliasingQuality);
			ADD_CHILD_TO_COLLECTION(AntiAliasingQuality, Graphics);
		}
		// ViewDistanceQuality
		{	
			UListSettingDataObjectInteger* ViewDistanceQuality = NewObject<UListSettingDataObjectInteger>();
			ViewDistanceQuality->SetDataID(FName(TEXT("ViewDistanceQuality")));
			ViewDistanceQuality->SetDataDisplayName(FText::FromString(TEXT("View Distance")));
			ViewDistanceQuality->SetDescriptionRichText(GET_DESCRIPTION_FOR_KEY("ViewDistanceDescKey"));
			ViewDistanceQuality->SetbShouldApplySettingChangeImmediately(true);
			ViewDistanceQuality->AddIntegerSetting(0, FText::FromString(TEXT("Near")));
			ViewDistanceQuality->AddIntegerSetting(1, FText::FromString(TEXT("Medium")));
			ViewDistanceQuality->AddIntegerSetting(2, FText::FromString(TEXT("Far")));
			ViewDistanceQuality->AddIntegerSetting(3, FText::FromString(TEXT("Very Far")));
			ViewDistanceQuality->AddIntegerSetting(4, FText::FromString(TEXT("Cinematic")));
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(ViewDistanceQuality);
			ViewDistanceQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(ViewDistanceQuality);
			ADD_CHILD_TO_COLLECTION(ViewDistanceQuality, Graphics);
		}
		// TextureQuality
		{	
			UListSettingDataObjectInteger* TextureQuality = NewObject<UListSettingDataObjectInteger>();
			TextureQuality->SetDataID(FName(TEXT("TextureQuality")));
			TextureQuality->SetDataDisplayName(FText::FromString(TEXT("Texture Quality")));
			TextureQuality->SetDescriptionRichText(GET_DESCRIPTION_FOR_KEY("TextureQualityDescKey"));
			TextureQuality->SetbShouldApplySettingChangeImmediately(true);
			TextureQuality->AddIntegerSetting(0, FText::FromString(TEXT("Low")));
			TextureQuality->AddIntegerSetting(1, FText::FromString(TEXT("Medium")));
			TextureQuality->AddIntegerSetting(2, FText::FromString(TEXT("High")));
			TextureQuality->AddIntegerSetting(3, FText::FromString(TEXT("Epic")));
			TextureQuality->AddIntegerSetting(4, FText::FromString(TEXT("Cinematic")));
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(TextureQuality);
			TextureQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(TextureQuality);
			ADD_CHILD_TO_COLLECTION(TextureQuality, Graphics);
		}
		// VisualEffectQuality
		{	
			UListSettingDataObjectInteger* VisualEffectQuality = NewObject<UListSettingDataObjectInteger>();
			VisualEffectQuality->SetDataID(FName(TEXT("VisualEffectQuality")));
			VisualEffectQuality->SetDataDisplayName(FText::FromString(TEXT("Visual Effect Quality")));
			VisualEffectQuality->SetDescriptionRichText(GET_DESCRIPTION_FOR_KEY("VisualEffectQualityDescKey"));
			VisualEffectQuality->SetbShouldApplySettingChangeImmediately(true);
			VisualEffectQuality->AddIntegerSetting(0, FText::FromString(TEXT("Low")));
			VisualEffectQuality->AddIntegerSetting(1, FText::FromString(TEXT("Medium")));
			VisualEffectQuality->AddIntegerSetting(2, FText::FromString(TEXT("High")));
			VisualEffectQuality->AddIntegerSetting(3, FText::FromString(TEXT("Epic")));
			VisualEffectQuality->AddIntegerSetting(4, FText::FromString(TEXT("Cinematic")));
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(VisualEffectQuality);
			VisualEffectQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(VisualEffectQuality);
			ADD_CHILD_TO_COLLECTION(VisualEffectQuality, Graphics);
		}
		// ReflectionQuality
		{	
			UListSettingDataObjectInteger* ReflectionQuality = NewObject<UListSettingDataObjectInteger>();
			ReflectionQuality->SetDataID(FName(TEXT("ReflectionQuality")));
			ReflectionQuality->SetDataDisplayName(FText::FromString(TEXT("Reflection Quality")));
			ReflectionQuality->SetDescriptionRichText(GET_DESCRIPTION_FOR_KEY("ReflectionQualityDescKey"));
			ReflectionQuality->SetbShouldApplySettingChangeImmediately(true);
			ReflectionQuality->AddIntegerSetting(0, FText::FromString(TEXT("Low")));
			ReflectionQuality->AddIntegerSetting(1, FText::FromString(TEXT("Medium")));
			ReflectionQuality->AddIntegerSetting(2, FText::FromString(TEXT("High")));
			ReflectionQuality->AddIntegerSetting(3, FText::FromString(TEXT("Epic")));
			ReflectionQuality->AddIntegerSetting(4, FText::FromString(TEXT("Cinematic")));
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(ReflectionQuality);
			ReflectionQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(ReflectionQuality);
			ADD_CHILD_TO_COLLECTION(ReflectionQuality, Graphics);
		}
		// PostProcessingQuality
		{	
			UListSettingDataObjectInteger* PostProcessingQuality = NewObject<UListSettingDataObjectInteger>();
			PostProcessingQuality->SetDataID(FName(TEXT("PostProcessingQuality")));
			PostProcessingQuality->SetDataDisplayName(FText::FromString(TEXT("Post Processing Quality")));
			PostProcessingQuality->SetDescriptionRichText(GET_DESCRIPTION_FOR_KEY("PostProcessingQualityDescKey"));
			PostProcessingQuality->SetbShouldApplySettingChangeImmediately(true);
			PostProcessingQuality->AddIntegerSetting(0, FText::FromString(TEXT("Low")));
			PostProcessingQuality->AddIntegerSetting(1, FText::FromString(TEXT("Medium")));
			PostProcessingQuality->AddIntegerSetting(2, FText::FromString(TEXT("High")));
			PostProcessingQuality->AddIntegerSetting(3, FText::FromString(TEXT("Epic")));
			PostProcessingQuality->AddIntegerSetting(4, FText::FromString(TEXT("Cinematic")));
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(PostProcessingQuality);
			PostProcessingQuality->AddEditDependencyData(CachedOverallScalabilityLevel);
			CachedOverallScalabilityLevel->AddEditDependencyData(PostProcessingQuality);
			ADD_CHILD_TO_COLLECTION(PostProcessingQuality, Graphics);
		}
	}
	// Advanced Graphics Category
	{
		INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(AdvancedGraphics);
		ADD_CHILD_TO_COLLECTION(AdvancedGraphics, Video);
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
			INIT_CHILD_STRING_DATA_AND_SET_ID_NAME(FrameRateLimit);
			FrameRateLimit->AddSettingEntry(LexToString(30.0f), FText::FromString(TEXT("30 FPS")));
			FrameRateLimit->AddSettingEntry(LexToString(60.0f), FText::FromString(TEXT("60 FPS")));
			FrameRateLimit->AddSettingEntry(LexToString(90.0f), FText::FromString(TEXT("90 FPS")));
			FrameRateLimit->AddSettingEntry(LexToString(120.0f), FText::FromString(TEXT("120 FPS")));
			FrameRateLimit->AddSettingEntry(LexToString(0.0f), FText::FromString(TEXT("No Limit")));
			FrameRateLimit->SetDefaultValueFromString(LexToString(0.0f));

			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(FrameRateLimit);
			FrameRateLimit->SetDescriptionRichText(GET_DESCRIPTION_FOR_KEY("FrameRateLimitDescKey"));
			ADD_CHILD_TO_COLLECTION(FrameRateLimit, AdvancedGraphics);
		}
	}
}

void USettingDataRegistry::InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer)
{
	INIT_COLLECTION_TAB(Control);
	
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
		ADD_CHILD_TO_COLLECTION(Keyboard_And_Mouse, Control);
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
				{
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
		ADD_CHILD_TO_COLLECTION(GamepadCategory, Control);
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
