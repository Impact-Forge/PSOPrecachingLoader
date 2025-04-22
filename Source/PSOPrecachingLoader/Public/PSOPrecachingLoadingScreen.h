// Copyright 2024 - %Y Impact Forge LLC, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Styling/SlateTypes.h"

/**
 * 
 */
class PSOPRECACHINGLOADER_API SPSOPrecachingLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPSOPrecachingLoadingScreen) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
    
	// Tick function for updating the progress
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	// Progress tracking
	int32 TotalPSOs = 0;
	int32 RemainingPSOs = 0;
	float UpdateInterval = 0.5f;
	float TimeAccumulator = 0.0f;

	// Slate widgets
	TSharedPtr<class SProgressBar> ProgressBar;
	TSharedPtr<class STextBlock> ProgressTextBlock;
	TSharedPtr<class STextBlock> LoadingDescriptionTextBlock;

	// Update method
	void UpdatePSOProgress();
};
