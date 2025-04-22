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
