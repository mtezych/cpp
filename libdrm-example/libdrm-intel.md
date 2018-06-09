
# libDRM intel

```C
┌────────────────────────────────────┐
│          drm_intel_bufmgr          │
├────────────────────────────────────┤
│ drm_intel_bufmgr_gem_init          │
│ drm_intel_bufmgr_destroy           │
└────────────────────────────────────┘

┌────────────────────────────────────┐
│          drm_intel_context         │
├────────────────────────────────────┤
│ drm_intel_gem_context_create       │
│ drm_intel_gem_context_destroy      │
└────────────────────────────────────┘

┌────────────────────────────────────┐
│            drm_intel_bo            │
├────────────────────────────────────┤
│ drm_intel_bo_alloc                 │
│                                    │
│ drm_intel_bo_reference             │
│ drm_intel_bo_unreference           │
│                                    │
│ drm_intel_bo_map                   │
│ drm_intel_bo_unmap                 │
│                                    │
│ drm_intel_gem_bo_map_gtt           │
│ drm_intel_gem_bo_unmap_gtt         │
│                                    │
│ drm_intel_bo_set_tiling            │
│ drm_intel_bo_get_tiling            │
│                                    │
│ drm_intel_bo_flink                 │
│                                    │
│ drm_intel_bo_gem_export_to_prime   │
│ drm_intel_bo_gem_create_from_prime │
└────────────────────────────────────┘
```
