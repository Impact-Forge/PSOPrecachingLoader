# PSOPrecachingLoader
 A plugin to help manage PSO precaching with a custom loading screen

# PSO Precaching Configuration Guide

## Overview

This document explains the Pipeline State Object (PSO) precaching configuration implemented in your project's `DefaultEngine.ini`. PSO precaching helps eliminate shader compilation stuttering by precompiling all required shader configurations during loading screens instead of during gameplay.

## Configuration

Add the following to your `DefaultEngine.ini`:

```ini
[DevOptions.Shaders]
NeedsShaderStableKeys=true

[ShaderPipelineCache.CacheFile]
GameVersion=1000

[SystemSettings]
r.PSOPrecaching=1
r.PSOPrecache.Components=1
r.PSOPrecache.Resources=1
r.PSOPrecache.ProxyCreationWhenPSOReady=1
r.PSOPrecache.GlobalShaders=1
```

## Explanation of Settings

### Shader Configuration
- `NeedsShaderStableKeys=true` - Enables the generation of stable shader keys required for PSO caching.

### Pipeline Cache Management
- `GameVersion=1000` - Sets the version identifier for your PSO cache. Increment this when making significant content or shader changes that would invalidate existing PSO caches.

### PSO Precaching System Settings
- `r.PSOPrecaching=1` - Enables the global PSO precaching system.
- `r.PSOPrecache.Components=1` - Precaches PSOs used by components during loading.
- `r.PSOPrecache.Resources=1` - Precaches PSOs used by all resources (StaticMesh, SkinnedMesh, etc.)
- `r.PSOPrecache.ProxyCreationWhenPSOReady=1` - Waits for component proxy creation until all required PSOs are compiled.
- `r.PSOPrecache.GlobalShaders=1` - Precaches global compute and graphics PSOs during engine startup.

## Using the PSO Precaching Loader Plugin

The PSOPrecachingLoader plugin provides a clean way to integrate PSO precaching with your game's loading screens. This plugin ensures all shaders are properly compiled before the player begins gameplay, eliminating shader stuttering.

### Installation

1. Copy the `PSOPrecachingLoader` plugin folder to your project's `Plugins` directory
2. Add the plugin to your project's `.uproject` file:
   ```json
   "Plugins": [
     {
       "Name": "PSOPrecachingLoader",
       "Enabled": true
     }
   ]
   ```
3. Regenerate your project files and build the solution

### Integration Methods

#### Method 1: Using the Loading Screen Module

The plugin automatically registers a PSO loading screen that will display during level loads and wait for PSO compilation to complete:

```cpp
// No additional code required - the module handles loading screen registration
// See PSOPrecachingLoader.cpp for implementation details
```

#### Method 2: Using the PSO Precaching Actor

For more control, place the `APSOPrecachingLoaderActor` in your loading level:

1. Create a separate loading level in your project
2. Place the `APSOPrecachingLoaderActor` in this level
3. Configure the actor properties:
   - Set `LoadingWidgetClass` to your custom loading widget
   - Set `NextLevelToLoad` to the level that should load after PSO compilation

```cpp
// Example game mode code to start PSO precaching
void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // Find and use the PSO precaching actor
    APSOPrecachingLoaderActor* PSOActor = Cast<APSOPrecachingLoaderActor>(
        UGameplayStatics::GetActorOfClass(GetWorld(), APSOPrecachingLoaderActor::StaticClass()));
    
    if (PSOActor)
    {
        PSOActor->StartPSOPrecaching();
    }
}
```

#### Method 3: Custom Implementation

If you prefer a custom approach, you can implement PSO precaching directly:

```cpp
// In your loading screen widget or game instance
void WaitForPSOPrecaching()
{
    // Set PSO mode to fast during loading
    IConsoleVariable* PSOStartupMode = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ShaderPipelineCache.StartupMode"));
    if (PSOStartupMode)
    {
        PSOStartupMode->Set(1); // Fast mode
    }
    
    // Resume PSO batching if it was paused
    FShaderPipelineCache::ResumeBatching();
    
    // Check precaching progress periodically
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, 
        [this]() 
        { 
            int32 RemainingPSOs = FShaderPipelineCache::NumPrecompilesRemaining();
            if (RemainingPSOs <= 0)
            {
                // PSO compilation complete, proceed with game
                LoadNextLevel();
            }
        }, 
        0.5f, 
        true
    );
}
```

### Creating a Custom Loading Widget

The plugin includes `SPSOPrecachingLoadingScreen` that displays PSO compilation progress. You can:

1. Use it directly
2. Create a UMG widget that displays the progress information:
   ```cpp
   // In your UMG widget's Tick event
   void UMyLoadingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
   {
       Super::NativeTick(MyGeometry, InDeltaTime);
       
       int32 RemainingPSOs = FShaderPipelineCache::NumPrecompilesRemaining();
       float Progress = 1.0f - (static_cast<float>(RemainingPSOs) / EstimatedTotalPSOs);
       ProgressBar->SetPercent(FMath::Clamp(Progress, 0.0f, 1.0f));
   }
   ```

## Implementation Tips

1. **Loading Screen Integration**:
   - Implement a custom loading screen that waits for PSO precaching to complete.
   - Use `FShaderPipelineCache::NumPrecompilesRemaining()` to track progress.

2. **Performance Monitoring**:
   - Use `stat PSOPrecache` to view PSO precaching statistics in-game.
   - Enable detailed validation with `r.PSOPrecache.Validation=2` during development.

3. **Testing**:
   - Test with `-clearPSODriverCache` command line parameter to simulate first-time player experience.
   - Monitor frame time during gameplay to identify any remaining PSO-related hitches.

4. **Additional Settings to Consider**:
   - `r.PSOPrecache.ProxyCreationDelayStrategy` - Controls fallback behavior when PSOs aren't ready.
   - `r.pso.PrecompileThreadPoolPercentOfHardwareThreads` - Adjusts CPU resource allocation for PSO compilation.

## For Unpackaged Development

During development in the editor, consider setting these values in your console or a custom config file:

```
r.ShaderPipelineCache.Enabled=1
r.PSOPrecache.Validation=1
```

This will provide PSO precaching feedback during development sessions without the full validation overhead.
