
# libDRM AMDGPU

```C
┌────────────────────────────────────┐
│           amdgpu_device            │
├────────────────────────────────────┤
│ amdgpu_device_initialize           │
│ amdgpu_device_deinitialize         │
│                                    │
│ amdgpu_query_buffer_size_alignment │
│ amdgpu_query_firmware_version      │
│ amdgpu_query_crtc_from_id          │
│ amdgpu_query_info                  │
│ amdgpu_query_heap_info             │
│ amdgpu_query_gpu_info              │
│ amdgpu_query_gds_info              │
│ amdgpu_query_sensor_info           │
│ amdgpu_query_hw_ip_info            │
│ amdgpu_query_hw_ip_count           │
│                                    │
│ amdgpu_read_mm_registers           │
│                                    │
│ amdgpu_get_marketing_name          │
└────────────────────────────────────┘

┌────────────────────────────────────┐
│             amdgpu_bo              │
├────────────────────────────────────┤
│ amdgpu_bo_alloc                    │
│ amdgpu_bo_free                     │
│                                    │
│ amdgpu_bo_import                   │
│ amdgpu_bo_export                   │
│                                    │
│ amdgpu_create_bo_from_user_mem     │
│                                    │
│ amdgpu_bo_va_op                    │
│ amdgpu_bo_va_op_raw                │
│                                    │
│ amdgpu_bo_cpu_map                  │
│ amdgpu_bo_cpu_unmap                │
│                                    │
│ amdgpu_bo_wait_for_idle            │
│                                    │
│ amdgpu_bo_set_metadata             │
│ amdgpu_bo_query_info               │
└────────────────────────────────────┘

┌────────────────────────────────────┐
│           amdgpu_bo_list           │
├────────────────────────────────────┤
│ amdgpu_bo_list_create              │
│ amdgpu_bo_list_destroy             │
│                                    │
│ amdgpu_bo_list_update              │
└────────────────────────────────────┘

┌────────────────────────────────────┐
│           amdgpu_context           │
├────────────────────────────────────┤
│ amdgpu_cs_ctx_create               │
│ amdgpu_cs_ctx_free                 │
│                                    │
│ amdgpu_cs_submit                   │
│                                    │
│ amdgpu_cs_query_reset_state        │
└────────────────────────────────────┘

┌────────────────────────────────────┐
│             amdgpu_va              │
├────────────────────────────────────┤
│ amdgpu_va_range_alloc              │
│ amdgpu_va_range_free               │
│                                    │
│ amdgpu_va_range_query              │
└────────────────────────────────────┘

┌────────────────────────────────────┐
│          amdgpu_cs_fence           │
├────────────────────────────────────┤
│ amdgpu_cs_query_fence_status       │
│                                    │
│ amdgpu_cs_wait_fences              │
└────────────────────────────────────┘

┌────────────────────────────────────┐
│          amdgpu_semaphore          │
├────────────────────────────────────┤
│ amdgpu_cs_create_semaphore         │
│ amdgpu_cs_destroy_semaphore        │
│                                    │
│ amdgpu_cs_signal_semaphore         │
│                                    │
│ amdgpu_cs_wait_semaphore           │
└────────────────────────────────────┘
```
