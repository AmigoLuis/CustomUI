// Amigo Luis all rights reserved


#include "PlayerController/FrontEndPlayerController.h"

#include "UILogger.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/FrontendGameUserSettings.h"

void AFrontEndPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	TArray<AActor*> DefaultCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), FName("DefaultCamera"), DefaultCameras);
	for (AActor* Camera : DefaultCameras)
	{
		SetViewTarget(Camera); break; 
	}
	UFrontendGameUserSettings* Settings = UFrontendGameUserSettings::Get();
	CHECK_NULL_RETURN(Settings);
	if (Settings->GetLastCPUBenchmarkResult() == -1.0f || Settings->GetLastGPUBenchmarkResult() == -1.0f)
	{
		Settings->RunHardwareBenchmark();
		Settings->ApplyHardwareBenchmarkResults();
	}
}
