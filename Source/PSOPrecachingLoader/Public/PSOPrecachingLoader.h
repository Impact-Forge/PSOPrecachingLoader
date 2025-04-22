// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FPSOPrecachingLoaderModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** Register the PSO precaching loading screen */
	void RegisterPSOPrecachingLoadingScreen();

	/** Unregister the PSO precaching loading screen */
	void UnregisterPSOPrecachingLoadingScreen();

private:
	/** Handle to the loading screen */
	FDelegateHandle LoadingScreenHandle;
};
