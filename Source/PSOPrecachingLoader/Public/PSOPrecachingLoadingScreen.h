// Copyright 2024 - %Y Impact Forge LLC, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Widgets/SCompoundWidget.h"
#include "Styling/SlateTypes.h"
#include "PSOPrecachingLoadingScreen.generated.h"

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


UCLASS()
class PSOPRECACHINGLOADER_API UPSOPrecachingSlateWidget : public UWidget
{
	GENERATED_BODY()

public:
	UPSOPrecachingSlateWidget(const FObjectInitializer& ObjectInitializer);

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	// End of UWidget interface

private:
	TSharedPtr<SPSOPrecachingLoadingScreen> MySlateWidget;
};