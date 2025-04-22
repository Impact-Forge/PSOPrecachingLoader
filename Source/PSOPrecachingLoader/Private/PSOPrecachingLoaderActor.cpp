// Copyright 2024 - %Y Impact Forge LLC, All Rights Reserved.


#include "PSOPrecachingLoaderActor.h"

#include "Kismet/GameplayStatics.h"
#include "ShaderPipelineCache.h"
#include "Blueprint/UserWidget.h"

APSOPrecachingLoaderActor::APSOPrecachingLoaderActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APSOPrecachingLoaderActor::BeginPlay()
{
	Super::BeginPlay();

	// Create loading widget
	if (LoadingWidgetClass)
	{
		LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetClass);
		if (LoadingWidget)
		{
			LoadingWidget->AddToViewport();
		}
	}

	// Start PSO precaching
	StartPSOPrecaching();
}

void APSOPrecachingLoaderActor::StartPSOPrecaching()
{
	// Set PSO precaching to fast mode during loading
	IConsoleVariable* PSOStartupMode = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ShaderPipelineCache.StartupMode"));
	if (PSOStartupMode)
	{
		PSOStartupMode->Set(1); // Fast mode
	}

	// Resume PSO batching
	FShaderPipelineCache::ResumeBatching();

	// Set a timer to check PSO precaching progress
	FTimerHandle TimerHandle; // Declare a timer handle
	GetWorldTimerManager().SetTimer(
		TimerHandle, 
		[this]() 
		{ 
			this->OnPSOPrecachingComplete(); 
		}, 
		0.5f, 
		true
	);
}

void APSOPrecachingLoaderActor::OnPSOPrecachingComplete()
{
	// Check if PSO precaching is done
	int32 RemainingPSOs = FShaderPipelineCache::NumPrecompilesRemaining();
    
	if (RemainingPSOs <= 0)
	{
		// Stop the timer
		GetWorldTimerManager().ClearAllTimersForObject(this);

		// Load the next level
		LoadNextLevel();
	}
}

void APSOPrecachingLoaderActor::LoadNextLevel()
{
	if (!NextLevelToLoad.IsNone())
	{
		UGameplayStatics::OpenLevel(GetWorld(), NextLevelToLoad);
	}
}