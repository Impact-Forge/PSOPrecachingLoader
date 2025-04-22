// Copyright Epic Games, Inc. All Rights Reserved.

#include "PSOPrecachingLoader.h"

#include "LoadingScreenSettings.h"
#include "MoviePlayer.h"
#include "PSOPrecachingLoadingScreen.h"

#define LOCTEXT_NAMESPACE "FPSOPrecachingLoaderModule"

void FPSOPrecachingLoaderModule::StartupModule()
{
	// Register loading screen
	if (IsRunningGame())
	{
		RegisterPSOPrecachingLoadingScreen();
	}
}

void FPSOPrecachingLoaderModule::ShutdownModule()
{
	UnregisterPSOPrecachingLoadingScreen();
}

void FPSOPrecachingLoaderModule::RegisterPSOPrecachingLoadingScreen()
{
	if (FSlateApplication::IsInitialized())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 1.0f;
        
		// Create the widget directly - no IsLoadingMovie property
		LoadingScreen.WidgetLoadingScreen = SNew(SPSOPrecachingLoadingScreen);

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);

		IConsoleVariable* PSOStartupMode = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ShaderPipelineCache.StartupMode"));
		if (PSOStartupMode)
		{
			PSOStartupMode->Set(1); // Set Fast mode for PSO compilation
		}
	}


}

void FPSOPrecachingLoaderModule::UnregisterPSOPrecachingLoadingScreen()
{
	// GetMoviePlayer has a Shutdown method
	if (GetMoviePlayer()->IsMovieCurrentlyPlaying())
	{
		GetMoviePlayer()->Shutdown();
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPSOPrecachingLoaderModule, PSOPrecachingLoader)