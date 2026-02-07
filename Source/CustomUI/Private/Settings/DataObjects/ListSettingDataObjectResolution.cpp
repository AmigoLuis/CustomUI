// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectResolution.h"

#include "LogMacros.h"
#include "UILogger.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Settings/FrontendGameUserSettings.h"
#include "Settings/FSettingDataInteractionHelper.h"

void UListSettingDataObjectResolution::InitResolutionValue()
{
	TArray<FIntPoint> SupportedResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(SupportedResolutions);
	
	SupportedResolutions.Sort([](const FIntPoint& A, const FIntPoint& B)
	{
		return A.SizeSquared() < B.SizeSquared();
	});
	
	for (FIntPoint& SupportedResolution : SupportedResolutions)
	{
		// SupportedResolution is : X=2560 Y=1440
		PrintInLog(SYMBOL_NAME_TEXT(SupportedResolution)
			TEXT(" is : ") + SupportedResolution.ToString(), Display);
		AddSettingEntry(ResolutionToStringValue(SupportedResolution), ResolutionToDisplayText(SupportedResolution));
	}
	if (SupportedResolutions.Num() > 0)
	{
		MaxSupportedResolutions = ResolutionToStringValue(SupportedResolutions.Last());
	}
	SetDefaultValueFromString(MaxSupportedResolutions);
}

void UListSettingDataObjectResolution::OnInitializeDataObject()
{
	Super::OnInitializeDataObject();
	if (!TrySetTextAccordingToString(CurrentSettingNameString))
	{
		CurrentSettingNameText = ResolutionToDisplayText(UFrontendGameUserSettings::Get()->GetScreenResolution());
	}
}

FString UListSettingDataObjectResolution::ResolutionToStringValue(const FIntPoint& Resolution) const
{
	// Stored resolution is: (X=2560,Y=1440)
	return FString::Format(TEXT("(X={0},Y={1})"), {Resolution.X, Resolution.Y});
}

FText UListSettingDataObjectResolution::ResolutionToDisplayText(const FIntPoint& Resolution) const
{
	return FText::FromString(FString::Format(TEXT("{0} x {1}"), 
		{Resolution.X, Resolution.Y}));
}
