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
	
	if (WorldContext.OwningGameInstance != GetGameInstance()) return;
	SetTickableTickType(ETickableTickType::Conditional);
	bIsLoadingMap = true;
	TryUpdateLoadingScreen();
}

void ULoadingScreenInstanceSubsystem::PostLoadMap(UWorld* LoadedWorld)
{
	const FString& MapName = LoadedWorld == nullptr ? TEXT("NullWorld") : LoadedWorld->GetMapName();
	PrintInLog(TEXT("PostLoadMap: ") + MapName);
	if (LoadedWorld == nullptr || LoadedWorld->GetGameInstance() != GetGameInstance()) return;
	bIsLoadingMap = false;
}

void ULoadingScreenInstanceSubsystem::TryUpdateLoadingScreen()
{
	// 检查 进入游戏的过场动画 是否 还在运行，如果还在运行，需要等待其结束
	
	// 检查是否应该展示 游戏关卡加载界面
	if (true)
	{
		// 展示 游戏关卡加载界面
	}
	else
	{
		// 移除当前的加载界面
		// 通知加载结束
		// 禁用ticking
		SetTickableTickType(ETickableTickType::Never);
	}
	
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

void ULoadingScreenInstanceSubsystem::Tick(float DeltaTime)
{
	TryUpdateLoadingScreen();
}

ETickableTickType ULoadingScreenInstanceSubsystem::GetTickableTickType() const
{
	if (IsTemplate()) return ETickableTickType::Never;
	return ETickableTickType::Conditional;
}

bool ULoadingScreenInstanceSubsystem::IsTickable() const
{
	return GetGameInstance() && GetGameInstance()->GetGameViewportClient();
}

TStatId ULoadingScreenInstanceSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(ULoadingScreenInstanceSubsystem, STATGROUP_Tickables);
}

UWorld* ULoadingScreenInstanceSubsystem::GetTickableGameObjectWorld() const
{
	if (const UGameInstance* OwningGameInstance = GetGameInstance())
	{
		return OwningGameInstance->GetWorld();
	}
	return nullptr;
}
