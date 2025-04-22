// Copyright 2024 - %Y Impact Forge LLC, All Rights Reserved.


#include "PSOPrecachingLoadingScreen.h"

#include "MoviePlayer.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ShaderPipelineCache.h"
#include "Widgets/Notifications/SProgressBar.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPSOPrecachingLoadingScreen::Construct(const FArguments& InArgs)
{
    // Set initial PSO tracking
    TotalPSOs = 50000; // Estimated total PSOs, can be dynamically set
    RemainingPSOs = TotalPSOs;

    ChildSlot
    [
        SNew(SOverlay)
        // Background
        + SOverlay::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [
            SNew(SImage)
            .ColorAndOpacity(FLinearColor(0.05f, 0.05f, 0.05f, 1.0f)) // Dark background
        ]

        // Content
        + SOverlay::Slot()
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        [
            SNew(SVerticalBox)
            
            // Title
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 0, 0, 20)
            [
                SNew(STextBlock)
                .Text(INVTEXT("Preparing Game Environment"))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 24))
                .ColorAndOpacity(FLinearColor::White)
            ]

            // Progress Bar
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(100, 0)
            [
                SAssignNew(ProgressBar, SProgressBar)
                .Percent(0.0f)
                // Remove BorderImage if it's not resolving
                .BackgroundImage(FCoreStyle::Get().GetBrush("ProgressBar.Background"))
                .FillImage(FCoreStyle::Get().GetBrush("ProgressBar.Filled"))
            ]

            // Progress Text
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 10)
            [
                SAssignNew(ProgressTextBlock, STextBlock)
                .Text(INVTEXT("Initializing..."))
                .Font(FCoreStyle::GetDefaultFontStyle("Regular", 16))
                .ColorAndOpacity(FLinearColor::White)
                .Justification(ETextJustify::Center)
            ]

            // Description
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 10)
            [
                SAssignNew(LoadingDescriptionTextBlock, STextBlock)
                .Text(INVTEXT("Optimizing rendering for smooth gameplay..."))
                .Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
                .ColorAndOpacity(FLinearColor::Gray)
                .Justification(ETextJustify::Center)
            ]
        ]
    ];

    // Initialize progress
    UpdatePSOProgress();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPSOPrecachingLoadingScreen::UpdatePSOProgress()
{
    // Get current PSO precaching progress
    int32 CurrentRemainingPSOs = FShaderPipelineCache::NumPrecompilesRemaining();
    
    // Update progress
    float Progress = 1.0f - (static_cast<float>(CurrentRemainingPSOs) / static_cast<float>(TotalPSOs));
    Progress = FMath::Clamp(Progress, 0.0f, 1.0f);

    // Update UI elements
    if (ProgressBar.IsValid())
    {
        ProgressBar->SetPercent(Progress);
    }

    if (ProgressTextBlock.IsValid())
    {
        int32 CompiledPSOs = TotalPSOs - CurrentRemainingPSOs;
        ProgressTextBlock->SetText(FText::Format(
            INVTEXT("Precaching PSOs: {0} / {1}"), 
            CompiledPSOs, 
            TotalPSOs
        ));
    }

    // Store current remaining PSOs for the next update
    RemainingPSOs = CurrentRemainingPSOs;
}

void SPSOPrecachingLoadingScreen::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
    
    // Accumulate time for updates
    TimeAccumulator += InDeltaTime;
    
    // Update progress at regular intervals
    if (TimeAccumulator >= UpdateInterval)
    {
        UpdatePSOProgress();
        TimeAccumulator = 0.0f;
        
        // If precaching is complete, you might want to signal completion
        if (RemainingPSOs <= 0)
        {
            // Notify game mode/level that loading is complete
            // You can use delegates or other mechanisms to communicate this
            if (LoadingDescriptionTextBlock.IsValid())
            {
                LoadingDescriptionTextBlock->SetText(INVTEXT("Optimization complete! Loading game..."));
            }
        }
    }

    if (FShaderPipelineCache::NumPrecompilesRemaining() <= 0)
    {
        // Signal loading is complete if you're using PlaybackType::WaitForManualStop
        GetMoviePlayer()->StopMovie();
    }
}
