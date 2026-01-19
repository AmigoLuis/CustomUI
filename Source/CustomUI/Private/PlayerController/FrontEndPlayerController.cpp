// Amigo Luis all rights reserved


#include "PlayerController/FrontEndPlayerController.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

void AFrontEndPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	TArray<AActor*> DefaultCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), FName("DefaultCamera"), DefaultCameras);
	for (AActor* Camera : DefaultCameras)
	{
		SetViewTarget(Camera); break; 
	}
}
