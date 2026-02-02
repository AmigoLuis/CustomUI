// Amigo Luis all rights reserved


#include "Subsystems/LoadingScreenInstanceSubsystem.h"

#include "UILogger.h"

bool ULoadingScreenInstanceSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UGameInstance* GameInstance = Cast<UGameInstance>(Outer);
	CHECK_NULL_RETURN_VALUE(GameInstance, false);
	if (!GameInstance->IsDedicatedServerInstance())
	{
		TArray<UClass*> DerivedClasses;
		GetDerivedClasses(GetClass(), DerivedClasses);
		return DerivedClasses.IsEmpty();
	}
	return false;
}

void ULoadingScreenInstanceSubsystem::PreLoadMapWithContext(const FWorldContext& WorldContext, const FString& MapName)
{
	PrintInLog(TEXT("PreLoadMap: ") + MapName);
}

void ULoadingScreenInstanceSubsystem::PostLoadMap(UWorld* LoadedWorld)
{
	const FString& MapName = LoadedWorld == nullptr ? TEXT("NullWorld") : LoadedWorld->GetMapName();
	PrintInLog(TEXT("PostLoadMap: ") + MapName);
}

void ULoadingScreenInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, 
		&ULoadingScreenInstanceSubsystem::PreLoadMapWithContext);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, 
		&ULoadingScreenInstanceSubsystem::PostLoadMap);
}

void ULoadingScreenInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}
