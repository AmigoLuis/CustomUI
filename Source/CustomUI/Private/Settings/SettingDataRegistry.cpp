// Amigo Luis all rights reserved


#include "Settings/SettingDataRegistry.h"

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
#define INIT_CHILD_SCALAR_DATA_AND_SET_ID_NAME(ChileName) \
UListSettingDataObjectScalar* ChileName = NewObject<UListSettingDataObjectScalar>();\
ChileName->SetDataID(FName(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetDataDisplayName(FText::FromString(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetbShouldApplySettingChangeImmediately(true);

#undef INIT_CHILD_ENUM_DATA_AND_SET_ID_NAME
#define INIT_CHILD_ENUM_DATA_AND_SET_ID_NAME(ChileName) \
UListSettingDataObjectStringEnum* ChileName = NewObject<UListSettingDataObjectStringEnum>();\
ChileName->SetDataID(FName(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetDataDisplayName(FText::FromString(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetbShouldApplySettingChangeImmediately(true);

#undef INIT_CHILD_STRING_BOOL_DATA_AND_SET_ID_NAME
#define INIT_CHILD_STRING_BOOL_DATA_AND_SET_ID_NAME(ChileName) \
UListSettingDataObjectStringBool* ChileName = NewObject<UListSettingDataObjectStringBool>();\
ChileName->SetDataID(FName(SYMBOL_NAME_TEXT(ChileName)));\
ChileName->SetDataDisplayName(FText::FromString(SYMBOL_NAME_TEXT(ChileName)));\
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
	LOG_STRING_PTR(InCollectionID.ToString());
	CHECK_NULL_RETURN_VALUE(FoundCollectionPtr, TArray<UListSettingDataObjectBase*>());
	TArray<UListSettingDataObjectBase*> AllChildItems;
	GetAllChildRecursivelyInOrder(*FoundCollectionPtr, AllChildItems);
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
		// Overall Volume
		{
			INIT_CHILD_SCALAR_DATA_AND_SET_ID_NAME(OverallVolume);
			OverallVolume->SetDescriptionRichText(FText::FromString(
				TEXT("Overall Volume Currently is not affecting real game sound volume")));
			OverallVolume->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.0f, 2.0f));
			OverallVolume->SetSliderStepSize(0.01f);
			OverallVolume->SetDefaultValueFromString(LexToString(1.0f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolume->SetDisplayFormattingOptions(UListSettingDataObjectScalar::NoDecimal());// NoDecimal:50% // One Decimal:50.5%
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(OverallVolume);
			ADD_CHILD_TO_COLLECTION(OverallVolume, Volume);
		}
		// Music Volume
		{
			INIT_CHILD_SCALAR_DATA_AND_SET_ID_NAME(MusicVolume);
			MusicVolume->SetDescriptionRichText(FText::FromString(
				TEXT("Music Volume Currently is not affecting real game sound volume")));
			MusicVolume->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.0f, 2.0f));
			MusicVolume->SetSliderStepSize(0.01f);
			MusicVolume->SetDefaultValueFromString(LexToString(1.0f));
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolume->SetDisplayFormattingOptions(UListSettingDataObjectScalar::NoDecimal());// NoDecimal:50% // One Decimal:50.5%
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(MusicVolume);
			ADD_CHILD_TO_COLLECTION(MusicVolume, Volume);
		}
		// SoundFX Volume
		{
			INIT_CHILD_SCALAR_DATA_AND_SET_ID_NAME(SoundFXVolume);
			SoundFXVolume->SetDescriptionRichText(FText::FromString(
				TEXT("SoundFX Volume Currently is not affecting real game sound volume")));
			SoundFXVolume->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			SoundFXVolume->SetOutputValueRange(TRange<float>(0.0f, 2.0f));
			SoundFXVolume->SetSliderStepSize(0.01f);
			SoundFXVolume->SetDefaultValueFromString(LexToString(1.0f));
			SoundFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			SoundFXVolume->SetDisplayFormattingOptions(UListSettingDataObjectScalar::NoDecimal());// NoDecimal:50% // One Decimal:50.5%
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(SoundFXVolume);
			ADD_CHILD_TO_COLLECTION(SoundFXVolume, Volume);
		}
	}
	// Sound 
	{
		INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(Sound);
		ADD_CHILD_TO_COLLECTION(Sound, Audio);
		// Allow Background Music
		{
			INIT_CHILD_STRING_BOOL_DATA_AND_SET_ID_NAME(AllowBackgroundMusic);
			AllowBackgroundMusic->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
			AllowBackgroundMusic->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			AllowBackgroundMusic->SetTrueAsDefaultValue();
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(AllowBackgroundMusic);
			ADD_CHILD_TO_COLLECTION(AllowBackgroundMusic, Sound);
		}
		// Use HDR Audio
		{
			INIT_CHILD_STRING_BOOL_DATA_AND_SET_ID_NAME(UseHDRAudio);
			UseHDRAudio->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
			UseHDRAudio->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			UseHDRAudio->SetFalseAsDefaultValue();
			ADD_CHILD_DYNAMIC_GETTER_AND_SETTER(UseHDRAudio);
			ADD_CHILD_TO_COLLECTION(UseHDRAudio, Sound);
		}
	}
}

#define DISABLED_RICH_TEXT_STYLE TEXT("Disabled")
void USettingDataRegistry::InitVideoCollectionTab()
{
	INIT_COLLECTION_TAB(Video);
	// Display
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
		UListSettingDataObjectStringEnum* CachedFullscreenMode;
		// Fullscreen Mode
		{
			INIT_CHILD_ENUM_DATA_AND_SET_ID_NAME(FullscreenMode);
			FullscreenMode->SetDescriptionRichText(FText::FromString(
				TEXT("Fullscreen Mode decides game's window is fullscreen or not.")));
			FullscreenMode->AddEnumOption(EWindowMode::Fullscreen, FText::FromString(TEXT("Fullscreen")));
			FullscreenMode->AddEnumOption(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("Borderless Fullscreen")));
			FullscreenMode->AddEnumOption(EWindowMode::Windowed, FText::FromString(TEXT("Windowed")));
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
			ScreenResolution->SetDescriptionRichText(FText::FromString(
				TEXT("Screen Resolution decides game's window scale.")));
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
	// Graphics
	{
		UListSettingDataObjectInteger* CachedOverallScalabilityLevel;
		INIT_CHILD_COLLECTION_DATA_AND_SET_ID_NAME(Graphics);
		ADD_CHILD_TO_COLLECTION(Graphics, Video);
		// Brightness
		{
			INIT_CHILD_SCALAR_DATA_AND_SET_ID_NAME(Brightness);
			Brightness->SetDescriptionRichText(FText::FromString(
				TEXT("This affects Brightness of the screen.")));
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
			OverallScalabilityLevel->SetDescriptionRichText(FText::FromString(
				TEXT("This affects Overall Video Quality.")));
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
			INIT_CHILD_SCALAR_DATA_AND_SET_ID_NAME(ResolutionScaleNormalized);
			ResolutionScaleNormalized->SetDescriptionRichText(FText::FromString(
				TEXT("This affects ResolutionScaleNormalized of the screen.")));
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
		// OverallScalabilityLevel
		{	
			UListSettingDataObjectInteger* GlobalIlluminationQuality = NewObject<UListSettingDataObjectInteger>();
			GlobalIlluminationQuality->SetDataID(FName(TEXT("GlobalIlluminationQuality")));
			GlobalIlluminationQuality->SetDataDisplayName(FText::FromString(TEXT("Global Illumination Quality")));
			GlobalIlluminationQuality->SetDescriptionRichText(FText::FromString(
				TEXT("This affects Global Illumination Quality.")));
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
	}
}

void USettingDataRegistry::InitControlCollectionTab()
{
	INIT_COLLECTION_TAB(Control);
}
#undef INIT_COLLECTION_TAB
