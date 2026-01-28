// Amigo Luis all rights reserved


#include "Settings/DataObjects/ListSettingDataObjectResolution.h"

#include "UILogger.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Settings/FSettingDataInteractionHelper.h"

void UListSettingDataObjectResolution::InitResolutionValue()
{
	TArray<FIntPoint> SupportedResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(SupportedResolutions);
	
	for (FIntPoint& SupportedResolution : SupportedResolutions)
	{
		// SupportedResolution is : X=2560 Y=1440
		PrintInLog(SYMBOL_NAME_TEXT(SupportedResolution)
			TEXT(" is : ") + SupportedResolution.ToString(), Display);
	}
}

void UListSettingDataObjectResolution::OnInitializeDataObject()
{
	Super::OnInitializeDataObject();
	if (DataDynamicGetter)
	{
		// Stored resolution is: is : (X=2560,Y=1440)
		PrintInLog(TEXT("Stored resolution is:")
			TEXT(" is : ") + DataDynamicGetter->GetValueAsString(), Display);
	}
}
