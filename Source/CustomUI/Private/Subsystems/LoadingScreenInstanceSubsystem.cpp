// Amigo Luis all rights reserved


#include "Subsystems/LoadingScreenInstanceSubsystem.h"

#include "PreLoadScreenManager.h"
#include "UILogger.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/LoadingScreenInterface.h"
#include "Settings/FrontEndLoadScreenSettings.h"

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
	if (IsPreloadScreenActive()) return;
	// 检查是否应该展示 游戏关卡加载界面
	if (ShouldShowLoadingScreen())
	{
		// 展示 游戏关卡加载界面
		TryShowLoadingScreen();
		OnLoadingReasonUpdated.Broadcast(CurrentLoadingReason);
	}
	else
	{
		// 移除当前的加载界面
		TryRemoveLoadingScreen();
		
		HoldLoadingScreenStartUpTime = -1.0f;
		// 通知加载结束
		// 禁用ticking
		SetTickableTickType(ETickableTickType::Never);
	}
	
}

bool ULoadingScreenInstanceSubsystem::IsPreloadScreenActive() const
{
	if (const FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get())
	{
		return PreLoadScreenManager->HasValidActivePreLoadScreen();
	}
	return false;
}

bool ULoadingScreenInstanceSubsystem::ShouldShowLoadingScreen()
{
	const UFrontEndLoadScreenSettings* LoadScreenSettings = GetDefault<UFrontEndLoadScreenSettings>();
	if (LoadScreenSettings == nullptr) return false;
	if (GIsEditor || !LoadScreenSettings->bShouldShowLoadingScreenInEditor) return false;
	
	const auto GameInstance = GetGameInstance();
	if (GameInstance == nullptr) return false;

	const auto GameViewportClient = GameInstance->GetGameViewportClient();
	if (GameViewportClient == nullptr) return false;
	if (IsLoadingScreenNeeded())
	{
		// TODO:搞清楚为什么需要加载界面时，要禁用 世界渲染 
		GameViewportClient->bDisableWorldRendering = true;
		return true;
	}
	CurrentLoadingReason = TEXT("Waiting For Texture Streaming");
	GameViewportClient->bDisableWorldRendering = false;
	const float CurrentTime = FPlatformTime::Seconds();
	if (HoldLoadingScreenStartUpTime < 0.0f)
	{
		HoldLoadingScreenStartUpTime = CurrentTime;
	}
	if (const float ElapsedTime = CurrentTime - HoldLoadingScreenStartUpTime;
		ElapsedTime < LoadScreenSettings->HoldLoadingScreenExtraSeconds)
	{
		// 经过的时间在计划的展示加载界面时间内，需要展示加载界面
		return true;
	}
	return false;
}

bool ULoadingScreenInstanceSubsystem::IsLoadingScreenNeeded()
{
	if (bIsLoadingMap)
	{
		CurrentLoadingReason = TEXT("Loading Level");
		return true;
	}
	const UGameInstance* OwningGameInstance = GetGameInstance();
	if (OwningGameInstance == nullptr) return false;
	const UWorld* OwningWorld = OwningGameInstance->GetWorld();
	if (OwningWorld == nullptr)
	{
		CurrentLoadingReason = TEXT("Initializing World");
		return true;
	}
	if (!OwningWorld->HasBegunPlay())
	{
		CurrentLoadingReason = TEXT("World hasn't begun play yet");
		return true;
	}
	if (!OwningWorld->GetFirstPlayerController())
	{
		CurrentLoadingReason = TEXT("Player Controller isn't valid yet");
		return true;
	}
	// we can also check if game states, player states or actors are ready
	return false;
}

void ULoadingScreenInstanceSubsystem::TryShowLoadingScreen()
{
	if (CachedCreatedLoadingScreen.IsValid()) return; // widget already created
	const UFrontEndLoadScreenSettings* LoadScreenSettings = GetDefault<UFrontEndLoadScreenSettings>();
	if (LoadScreenSettings == nullptr) return;
	const TSubclassOf<UUserWidget> LoadingScreenWidget = LoadScreenSettings->GetLoadingScreenWidgetClass();
	CHECK_NULL_RETURN(LoadingScreenWidget);
	UGameInstance* const GameInstance = GetGameInstance();
	CHECK_NULL_RETURN(GameInstance);
	UGameViewportClient* GameViewportClient = GameInstance->GetGameViewportClient();
	CHECK_NULL_RETURN(GameViewportClient);

	UUserWidget* const CreatedWidget = 
		UUserWidget::CreateWidgetInstance(*GameInstance, LoadingScreenWidget, NAME_None);
	CHECK_NULL_RETURN(CreatedWidget);
	CachedCreatedLoadingScreen = CreatedWidget->TakeWidget();
	GameViewportClient->AddViewportWidgetContent(CachedCreatedLoadingScreen.ToSharedRef(), INT32_MAX);
	NotifyLoadingScreenVisibilityChange(true);
}

void ULoadingScreenInstanceSubsystem::TryRemoveLoadingScreen()
{
	if (!CachedCreatedLoadingScreen.IsValid()) return;

	const UGameInstance* const GameInstance = GetGameInstance();
	CHECK_NULL_RETURN(GameInstance);
	UGameViewportClient* GameViewportClient = GameInstance->GetGameViewportClient();
	CHECK_NULL_RETURN(GameViewportClient);
	GameViewportClient->RemoveViewportWidgetContent(CachedCreatedLoadingScreen.ToSharedRef());
	
	CachedCreatedLoadingScreen.Reset();
	NotifyLoadingScreenVisibilityChange(false);
}

void ULoadingScreenInstanceSubsystem::NotifyLoadingScreenVisibilityChange(bool bIsVisible)
{
	const UGameInstance* const GameInstance = GetGameInstance();
	CHECK_NULL_RETURN(GameInstance);
	
	auto ExecuteIfImplementedLoadingScreenInterface = [bIsVisible](UObject* Object)
	{
		if (Object->Implements<ULoadingScreenInterface>())
		{
			if (bIsVisible)
			{
				ILoadingScreenInterface::Execute_OnLoadingScreenActivated(Object);
			}
			else
			{
				ILoadingScreenInterface::Execute_OnLoadingScreenDeactivated(Object);
			}
		}
	};
	
	for (const ULocalPlayer* ExistingLocalPlayer : GameInstance->GetLocalPlayers())
	{
		if (ExistingLocalPlayer == nullptr) continue;
		
		APlayerController* PlayerController = ExistingLocalPlayer->GetPlayerController(GameInstance->GetWorld());
		if (PlayerController == nullptr) continue;
		// if PlayerController implemented interface, notify LoadingScreenVisibility to it
		ExecuteIfImplementedLoadingScreenInterface(PlayerController);
		
		APawn* Pawn = PlayerController->GetPawn();
		if (Pawn == nullptr) continue;
		// if Pawn implemented interface, notify LoadingScreenVisibility to it
		ExecuteIfImplementedLoadingScreenInterface(Pawn);
	}
	// notify LoadingScreenVisibility to other actors in the world
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
