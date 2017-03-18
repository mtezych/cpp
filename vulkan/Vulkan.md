
# Vulkan Objects

```C
┌─────────────────────────────────────────────────────────┐
│                         VkLoader                        │
├─────────────────────────────────────────────────────────┤
│ vk Enumerate Instance Extension Properties              │
│ vk Enumerate Instance Layer     Properties              │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│                        VkInstance                       │
├─────────────────────────────────────────────────────────┤
│ vk Create    Instance                                   │
│ vk Destroy   Instance                                   │
│                                                         │
│ vk Get       Instance Proc Addr                         │
│                                                         │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                    VkSurfaceKHR                     │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Xlib    Surface KHR                      │ │
│ │ vk Create  Xcb     Surface KHR                      │ │
│ │ vk Create  Wayland Surface KHR                      │ │
│ │ vk Create  Android Surface KHR                      │ │
│ │ vk Create  Win32   Surface KHR                      │ │
│ │ vk Destroy         Surface KHR                      │ │
│ └─────────────────────────────────────────────────────┘ │
│                                                         │
│ vk Enumerate Physical Devices                           │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│                     VkPhysicalDevice                    │
├─────────────────────────────────────────────────────────┤
│ vk Get Physical Device                     Features     │
│                                                         │
│ vk Get Physical Device                     Properties   │
│ vk Get Physical Device Queue Family        Properties   │
│ vk Get Physical Device              Format Properties   │
│ vk Get Physical Device        Image Format Properties   │
│ vk Get Physical Device Sparse Image Format Properties   │
│ vk Get Physical Device Memory              Properties   │
│                                                         │
│ vk Get Physical Device Xlib    Presentation Support KHR │
│ vk Get Physical Device Xcb     Presentation Support KHR │
│ vk Get Physical Device Wayland Presentation Support KHR │
│ vk Get Physical Device Win32   Presentation Support KHR │
│                                                         │
│ vk Get Physical Device Surface Support       KHR        │
│ vk Get Physical Device Surface Capabilities  KHR        │
│ vk Get Physical Device Surface Formats       KHR        │
│ vk Get Physical Device Surface Present Modes KHR        │
│                                                         │
│ vk Enumerate Device Extension Properties                │
│ vk Enumerate Device Layer     Properties                │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│                       VkDevice                          │
├─────────────────────────────────────────────────────────┤
│ vk Create  Device                                       │
│ vk Destroy Device                                       │
│                                                         │
│ vk Get     Device Proc Addr                             │
│                                                         │
│ vk         Device Wait Idle                             │
│                                                         │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                       VkSemaphore                   │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Semaphore                                │ │
│ │ vk Destroy Semaphore                                │ │
│ └─────────────────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                         VkFence                     │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Fence                                    │ │
│ │ vk Destroy Fence                                    │ │
│ │                                                     │ │
│ │ vk Get     Fence Status                             │ │
│ │ vk Reset   Fences                                   │ │
│ │ vk WaitFor Fences                                   │ │
│ └─────────────────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                         VkEvent                     │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Event                                    │ │
│ │ vk Destroy Event                                    │ │
│ │                                                     │ │
│ │ vk Get     Event Status                             │ │
│ │ vk Reset   Event                                    │ │
│ │ vk Set     Event                                    │ │
│ └─────────────────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                     VkQueryPool                     │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  QueryPool                                │ │
│ │ vk Destroy QueryPool                                │ │
│ │                                                     │ │
│ │ vk Get     QueryPool Results                        │ │
│ └─────────────────────────────────────────────────────┘ │
│                                                         │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                      VkRenderPass                   │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Render Pass                              │ │
│ │ vk Destroy Render Pass                              │ │
│ │                                                     │ │
│ │ vk Get     Render Area Granularity                  │ │
│ └─────────────────────────────────────────────────────┘ │
│                                                         │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                     VkFramebuffer                   │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Framebuffer                              │ │
│ │ vk Destroy Framebuffer                              │ │
│ └─────────────────────────────────────────────────────┘ │
│                                                         │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                     VkShaderModule                  │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Shader Module                            │ │
│ │ vk Destroy Shader Module                            │ │
│ └─────────────────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                    VkPipelineCache                  │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Pipeline Cache                           │ │
│ │ vk Destroy Pipeline Cache                           │ │
│ │                                                     │ │
│ │ vk Get     Pipeline Cache Data                      │ │
│ │ vk Merge   Pipeline Caches                          │ │
│ └─────────────────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                    VkPipelineLayout                 │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Pipeline Layout                          │ │
│ │ vk Destroy Pipeline Layout                          │ │
│ └─────────────────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                    VkPipeline                       │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create Graphics Pipelines                        │ │
│ │ vk Create Compute  Pipelines                        │ │
│ │ vk Destroy         Pipeline                         │ │
│ └─────────────────────────────────────────────────────┘ │
│                                                         │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                  VkDeviceMemory                     │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Allocate   Memory                                │ │
│ │ vk Free       Memory                                │ │
│ │                                                     │ │
│ │ vk Map        Memory                                │ │
│ │ vk Unmap      Memory                                │ │
│ │                                                     │ │
│ │ vk Get Device Memory Commitment                     │ │
│ │                                                     │ │
│ │ vk Flush      Mapped Memory Ranges                  │ │
│ │ vk Invalidate Mapped Memory Ranges                  │ │
│ └─────────────────────────────────────────────────────┘ │
│                                                         │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                     VkBufferView                    │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Buffer View                              │ │
│ │ vk Destroy Buffer View                              │ │
│ └─────────────────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                     VkBuffer                        │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Buffer                                   │ │
│ │ vk Destroy Buffer                                   │ │
│ │                                                     │ │
│ │ vk Get     Buffer Memory Requirements               │ │
│ │                                                     │ │
│ │ vk Bind    Buffer Memory                            │ │
│ └─────────────────────────────────────────────────────┘ │
│                                                         │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                      VkImageView                    │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Image View                               │ │
│ │ vk Destroy Image View                               │ │
│ └─────────────────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                      VkImage                        │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Image                                    │ │
│ │ vk Destroy Image                                    │ │
│ │                                                     │ │
│ │ vk Get     Image        Memory Requirements         │ │
│ │ vk Get     Image Sparse Memory Requirements         │ │
│ │                                                     │ │
│ │ vk Bind    Image        Memory                      │ │
│ │                                                     │ │
│ │ vk Get     Image Subresource Layout                 │ │
│ └─────────────────────────────────────────────────────┘ │
│                                                         │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                       VkSampler                     │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Sampler                                  │ │
│ │ vk Destroy Sampler                                  │ │
│ └─────────────────────────────────────────────────────┘ │
│                                                         │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                 VkDescriptorSetLayout               │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create   Descriptor Set Layout                   │ │
│ │ vk Destroy  Descriptor Set Layout                   │ │
│ └─────────────────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                 VkDescriptorPool                    │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create   Descriptor Pool                         │ │
│ │ vk Destroy  Descriptor Pool                         │ │
│ │ vk Reset    Descriptor Pool                         │ │
│ └─────────────────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                 VkDescriptorSet                     │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Allocate Descriptor Sets                         │ │
│ │ vk Free     Descriptor Sets                         │ │
│ │ vk Update   Descriptor Sets                         │ │
│ └─────────────────────────────────────────────────────┘ │
│                                                         │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                   VkSwapchainKHR                    │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create        Swapchain        KHR               │ │
│ │ vk Destroy       Swapchain        KHR               │ │
│ │ vk Get           Swapchain Images KHR               │ │
│ │ vk Acquire Next            Image  KHR               │ │
│ └─────────────────────────────────────────────────────┘ │
│                                                         │
│ ┌─────────────────────────────────────────────────────┐ │
│ │                     VkCommandPool                   │ │
│ ├─────────────────────────────────────────────────────┤ │
│ │ vk Create  Command Pool                             │ │
│ │ vk Destroy Command Pool                             │ │
│ │                                                     │ │
│ │ vk Reset   Command Pool                             │ │
│ └─────────────────────────────────────────────────────┘ │
│                                                         │
│ vk Get Device Queue                                     │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│                         VkQueue                         │
├─────────────────────────────────────────────────────────┤
│ vk Queue Submit                                         │
│ vk Queue Wait Idle                                      │
│ vk Queue Bind Sparse                                    │
│                                                         │
│ vk Queue Present KHR                                    │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│                   VkCommandBuffer                       │
├─────────────────────────────────────────────────────────┤
│ vk Allocate Command Buffers                             │
│ vk Free     Command Buffers                             │
│                                                         │
│ vk Begin    Command Buffer                              │
│ vk End      Command Buffer                              │
│ vk Reset    Command Buffer                              │
│                                                         │
│ vk Cmd Set Viewport                                     │
│ vk Cmd Set Scissor                                      │
│ vk Cmd Set Line Width                                   │
│ vk Cmd Set Depth Bias                                   │
│ vk Cmd Set Depth Bounds                                 │
│ vk Cmd Set Blend Constants                              │
│ vk Cmd Set Stencil Compare Mask                         │
│ vk Cmd Set Stencil Write   Mask                         │
│ vk Cmd Set Stencil Reference                            │
│                                                         │
│ vk Cmd Bind Pipeline                                    │
│ vk Cmd Bind Descriptor Sets                             │
│ vk Cmd Bind Index  Buffer                               │
│ vk Cmd Bind Vertex Buffers                              │
│                                                         │
│ vk Cmd Draw                                             │
│ vk Cmd Draw Indexed                                     │
│ vk Cmd Draw         Indirect                            │
│ vk Cmd Draw Indexed Indirect                            │
│                                                         │
│ vk Cmd Dispatch                                         │
│ vk Cmd Dispatch     Indirect                            │
│                                                         │
│ vk Cmd Clear Color         Image                        │
│ vk Cmd Clear Depth Stencil Image                        │
│ vk Cmd Clear Attachments                                │
│                                                         │
│ vk Cmd Resolve Image                                    │
│                                                         │
│ vk Cmd Blit Image                                       │
│                                                         │
│ vk Cmd Copy Image                                       │
│ vk Cmd Copy Buffer                                      │
│                                                         │
│ vk Cmd Copy Image  To Buffer                            │
│ vk Cmd Copy Buffer To Image                             │
│                                                         │
│ vk Cmd Update Buffer                                    │
│ vk Cmd Fill   Buffer                                    │
│                                                         │
│ vk Cmd Set   Event                                      │
│ vk Cmd Reset Event                                      │
│ vk Cmd Wait  Events                                     │
│                                                         │
│ vk Cmd Begin Query                                      │
│ vk Cmd End   Query                                      │
│ vk Cmd Copy  Query Pool Results                         │
│ vk Cmd Reset Query Pool                                 │
│                                                         │
│ vk Cmd Write Timestamp                                  │
│                                                         │
│ vk Cmd Pipeline Barrier                                 │
│                                                         │
│ vk Cmd Begin Render Pass                                │
│ vk Cmd Next      Subpass                                │
│ vk Cmd End   Render Pass                                │
│                                                         │
│ vk Cmd Push Constants                                   │
│                                                         │
│ vk Cmd Execute Commands                                 │
└─────────────────────────────────────────────────────────┘
```

# Vulkan Functions

```C
VkResult           vkEnumerateInstanceExtensionProperties
(
    const char*                            pLayerName,
    uint32_t*                              pPropertyCount,
    VkExtensionProperties*                 pProperties
);

VkResult           vkEnumerateInstanceLayerProperties
(
    uint32_t*                              pPropertyCount,
    VkLayerProperties*                     pProperties
);


VkResult           vkCreateInstance
(
    const VkInstanceCreateInfo*            pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkInstance*                            pInstance
);

void               vkDestroyInstance
(
    VkInstance                             instance,
    const VkAllocationCallbacks*           pAllocator
);

PFN_vkVoidFunction vkGetInstanceProcAddr
(
    VkInstance                             instance,
    const char*                            pName
);

VkResult           vkEnumeratePhysicalDevices
(
    VkInstance                             instance,
    uint32_t*                              pPhysicalDeviceCount,
    VkPhysicalDevice*                      pPhysicalDevices
);


void               vkGetPhysicalDeviceFeatures
(
    VkPhysicalDevice                       physicalDevice,
    VkPhysicalDeviceFeatures*              pFeatures
);

void               vkGetPhysicalDeviceProperties
(
    VkPhysicalDevice                       physicalDevice,
    VkPhysicalDeviceProperties*            pProperties
);

void               vkGetPhysicalDeviceQueueFamilyProperties
(
    VkPhysicalDevice                       physicalDevice,
    uint32_t*                              pQueueFamilyPropertyCount,
    VkQueueFamilyProperties*               pQueueFamilyProperties
);

void               vkGetPhysicalDeviceFormatProperties
(
    VkPhysicalDevice                       physicalDevice,
    VkFormat                               format,
    VkFormatProperties*                    pFormatProperties
);

VkResult           vkGetPhysicalDeviceImageFormatProperties
(
    VkPhysicalDevice                       physicalDevice,
    VkFormat                               format,
    VkImageType                            type,
    VkImageTiling                          tiling,
    VkImageUsageFlags                      usage,
    VkImageCreateFlags                     flags,
    VkImageFormatProperties*               pImageFormatProperties
);

void               vkGetPhysicalDeviceSparseImageFormatProperties
(
    VkPhysicalDevice                       physicalDevice,
    VkFormat                               format,
    VkImageType                            type,
    VkSampleCountFlagBits                  samples,
    VkImageUsageFlags                      usage,
    VkImageTiling                          tiling,
    uint32_t*                              pPropertyCount,
    VkSparseImageFormatProperties*         pProperties
);

void               vkGetPhysicalDeviceMemoryProperties
(
    VkPhysicalDevice                       physicalDevice,
    VkPhysicalDeviceMemoryProperties*      pMemoryProperties
);

VkResult           vkEnumerateDeviceExtensionProperties
(
    VkPhysicalDevice                       physicalDevice,
    const char*                            pLayerName,
    uint32_t*                              pPropertyCount,
    VkExtensionProperties*                 pProperties
);

VkResult           vkEnumerateDeviceLayerProperties
(
    VkPhysicalDevice                       physicalDevice,
    uint32_t*                              pPropertyCount,
    VkLayerProperties*                     pProperties
);


VkResult           vkCreateDevice
(
    VkPhysicalDevice                       physicalDevice,
    const VkDeviceCreateInfo*              pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkDevice*                              pDevice
);

void               vkDestroyDevice
(
    VkDevice                               device,
    const VkAllocationCallbacks*           pAllocator
);

PFN_vkVoidFunction vkGetDeviceProcAddr
(
    VkDevice                               device,
    const char*                            pName
);

VkResult           vkDeviceWaitIdle
(
    VkDevice                               device
);


VkResult           vkCreateSemaphore
(
    VkDevice                               device,
    const VkSemaphoreCreateInfo*           pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkSemaphore*                           pSemaphore
);

void               vkDestroySemaphore
(
    VkDevice                               device,
    VkSemaphore                            semaphore,
    const VkAllocationCallbacks*           pAllocator
);


VkResult           vkCreateFence
(
    VkDevice                               device,
    const VkFenceCreateInfo*               pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkFence*                               pFence
);

void               vkDestroyFence
(
    VkDevice                               device,
    VkFence                                fence,
    const VkAllocationCallbacks*           pAllocator
);

VkResult           vkGetFenceStatus
(
    VkDevice                               device,
    VkFence                                fence
);

VkResult           vkResetFences
(
    VkDevice                               device,
    uint32_t                               fenceCount,
    const VkFence*                         pFences
);

VkResult           vkWaitForFences
(
    VkDevice                               device,
    uint32_t                               fenceCount,
    const VkFence*                         pFences,
    VkBool32                               waitAll,
    uint64_t                               timeout
);


VkResult           vkCreateEvent
(
    VkDevice                               device,
    const VkEventCreateInfo*               pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkEvent*                               pEvent
);

void               vkDestroyEvent
(
    VkDevice                               device,
    VkEvent                                event,
    const VkAllocationCallbacks*           pAllocator
);

VkResult           vkGetEventStatus
(
    VkDevice                               device,
    VkEvent                                event
);

VkResult           vkResetEvent
(
    VkDevice                               device,
    VkEvent                                event
);

VkResult           vkSetEvent
(
    VkDevice                               device,
    VkEvent                                event
);


VkResult           vkCreateQueryPool
(
    VkDevice                               device,
    const VkQueryPoolCreateInfo*           pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkQueryPool*                           pQueryPool
);

void               vkDestroyQueryPool
(
    VkDevice                               device,
    VkQueryPool                            queryPool,
    const VkAllocationCallbacks*           pAllocator
);

VkResult           vkGetQueryPoolResults
(
    VkDevice                               device,
    VkQueryPool                            queryPool,
    uint32_t                               firstQuery,
    uint32_t                               queryCount,
    size_t                                 dataSize,
    void*                                  pData,
    VkDeviceSize                           stride,
    VkQueryResultFlags                     flags
);


VkResult           vkCreateRenderPass
(
    VkDevice                               device,
    const VkRenderPassCreateInfo*          pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkRenderPass*                          pRenderPass
);

void               vkDestroyRenderPass
(
    VkDevice                               device,
    VkRenderPass                           renderPass,
    const VkAllocationCallbacks*           pAllocator
);

void               vkGetRenderAreaGranularity
(
    VkDevice                               device,
    VkRenderPass                           renderPass,
    VkExtent2D*                            pGranularity
);


VkResult           vkCreateFramebuffer
(
    VkDevice                               device,
    const VkFramebufferCreateInfo*         pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkFramebuffer*                         pFramebuffer
);

void               vkDestroyFramebuffer
(
    VkDevice                               device,
    VkFramebuffer                          framebuffer,
    const VkAllocationCallbacks*           pAllocator
);


VkResult           vkCreateShaderModule
(
    VkDevice                               device,
    const VkShaderModuleCreateInfo*        pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkShaderModule*                        pShaderModule
);

void               vkDestroyShaderModule
(
    VkDevice                               device,
    VkShaderModule                         shaderModule,
    const VkAllocationCallbacks*           pAllocator
);


VkResult           vkCreatePipelineCache
(
    VkDevice                               device,
    const VkPipelineCacheCreateInfo*       pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkPipelineCache*                       pPipelineCache
);

void               vkDestroyPipelineCache
(
    VkDevice                               device,
    VkPipelineCache                        pipelineCache,
    const VkAllocationCallbacks*           pAllocator
);

VkResult           vkGetPipelineCacheData
(
    VkDevice                               device,
    VkPipelineCache                        pipelineCache,
    size_t*                                pDataSize,
    void*                                  pData
);

VkResult           vkMergePipelineCaches
(
    VkDevice                               device,
    VkPipelineCache                        dstCache,
    uint32_t                               srcCacheCount,
    const VkPipelineCache*                 pSrcCaches
);


VkResult           vkCreatePipelineLayout
(
    VkDevice                               device,
    const VkPipelineLayoutCreateInfo*      pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkPipelineLayout*                      pPipelineLayout
);

void               vkDestroyPipelineLayout
(
    VkDevice                               device,
    VkPipelineLayout                       pipelineLayout,
    const VkAllocationCallbacks*           pAllocator
);


VkResult           vkCreateGraphicsPipelines
(
    VkDevice                               device,
    VkPipelineCache                        pipelineCache,
    uint32_t                               createInfoCount,
    const VkGraphicsPipelineCreateInfo*    pCreateInfos,
    const VkAllocationCallbacks*           pAllocator,
    VkPipeline*                            pPipelines
);

VkResult           vkCreateComputePipelines
(
    VkDevice                               device,
    VkPipelineCache                        pipelineCache,
    uint32_t                               createInfoCount,
    const VkComputePipelineCreateInfo*     pCreateInfos,
    const VkAllocationCallbacks*           pAllocator,
    VkPipeline*                            pPipelines
);

void               vkDestroyPipeline
(
    VkDevice                               device,
    VkPipeline                             pipeline,
    const VkAllocationCallbacks*           pAllocator
);


VkResult           vkAllocateMemory
(
    VkDevice                               device,
    const VkMemoryAllocateInfo*            pAllocateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkDeviceMemory*                        pMemory
);

void               vkFreeMemory
(
    VkDevice                               device,
    VkDeviceMemory                         memory,
    const VkAllocationCallbacks*           pAllocator
);

VkResult           vkMapMemory
(
    VkDevice                               device,
    VkDeviceMemory                         memory,
    VkDeviceSize                           offset,
    VkDeviceSize                           size,
    VkMemoryMapFlags                       flags,
    void**                                 ppData
);

void               vkUnmapMemory
(
    VkDevice                               device,
    VkDeviceMemory                         memory
);

void               vkGetDeviceMemoryCommitment
(
    VkDevice                               device,
    VkDeviceMemory                         memory,
    VkDeviceSize*                          pCommittedMemoryInBytes
);

VkResult           vkFlushMappedMemoryRanges
(
    VkDevice                               device,
    uint32_t                               memoryRangeCount,
    const VkMappedMemoryRange*             pMemoryRanges
);

VkResult           vkInvalidateMappedMemoryRanges
(
    VkDevice                               device,
    uint32_t                               memoryRangeCount,
    const VkMappedMemoryRange*             pMemoryRanges
);


VkResult           vkCreateBufferView
(
    VkDevice                               device,
    const VkBufferViewCreateInfo*          pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkBufferView*                          pView
);

void               vkDestroyBufferView
(
    VkDevice                               device,
    VkBufferView                           bufferView,
    const VkAllocationCallbacks*           pAllocator
);


VkResult           vkCreateBuffer
(
    VkDevice                               device,
    const VkBufferCreateInfo*              pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkBuffer*                              pBuffer
);

void               vkDestroyBuffer
(
    VkDevice                               device,
    VkBuffer                               buffer,
    const VkAllocationCallbacks*           pAllocator
);

void               vkGetBufferMemoryRequirements
(
    VkDevice                               device,
    VkBuffer                               buffer,
    VkMemoryRequirements*                  pMemoryRequirements
);

VkResult           vkBindBufferMemory
(
    VkDevice                               device,
    VkBuffer                               buffer,
    VkDeviceMemory                         memory,
    VkDeviceSize                           memoryOffset
);


VkResult           vkCreateImageView
(
    VkDevice                               device,
    const VkImageViewCreateInfo*           pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkImageView*                           pView
);

void               vkDestroyImageView
(
    VkDevice                               device,
    VkImageView                            imageView,
    const VkAllocationCallbacks*           pAllocator
);


VkResult           vkCreateImage
(
    VkDevice                               device,
    const VkImageCreateInfo*               pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkImage*                               pImage
);

void               vkDestroyImage
(
    VkDevice                               device,
    VkImage                                image,
    const VkAllocationCallbacks*           pAllocator
);

void               vkGetImageMemoryRequirements
(
    VkDevice                               device,
    VkImage                                image,
    VkMemoryRequirements*                  pMemoryRequirements
);

void               vkGetImageSparseMemoryRequirements
(
    VkDevice                               device,
    VkImage                                image,
    uint32_t*                              pSparseMemoryRequirementCount,
    VkSparseImageMemoryRequirements*       pSparseMemoryRequirements
);

VkResult           vkBindImageMemory
(
    VkDevice                               device,
    VkImage                                image,
    VkDeviceMemory                         memory,
    VkDeviceSize                           memoryOffset
);

void               vkGetImageSubresourceLayout
(
    VkDevice                               device,
    VkImage                                image,
    const VkImageSubresource*              pSubresource,
    VkSubresourceLayout*                   pLayout
);


VkResult           vkCreateSampler
(
    VkDevice                               device,
    const VkSamplerCreateInfo*             pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkSampler*                             pSampler
);

void               vkDestroySampler
(
    VkDevice                               device,
    VkSampler                              sampler,
    const VkAllocationCallbacks*           pAllocator
);


VkResult           vkCreateDescriptorSetLayout
(
    VkDevice                               device,
    const VkDescriptorSetLayoutCreateInfo* pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkDescriptorSetLayout*                 pSetLayout
);

void               vkDestroyDescriptorSetLayout
(
    VkDevice                               device,
    VkDescriptorSetLayout                  descriptorSetLayout,
    const VkAllocationCallbacks*           pAllocator
);


VkResult           vkCreateDescriptorPool
(
    VkDevice                               device,
    const VkDescriptorPoolCreateInfo*      pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkDescriptorPool*                      pDescriptorPool
);

void               vkDestroyDescriptorPool
(
    VkDevice                               device,
    VkDescriptorPool                       descriptorPool,
    const VkAllocationCallbacks*           pAllocator
);

VkResult           vkResetDescriptorPool
(
    VkDevice                               device,
    VkDescriptorPool                       descriptorPool,
    VkDescriptorPoolResetFlags             flags
);


VkResult           vkAllocateDescriptorSets
(
    VkDevice                               device,
    const VkDescriptorSetAllocateInfo*     pAllocateInfo,
    VkDescriptorSet*                       pDescriptorSets
);

VkResult           vkFreeDescriptorSets
(
    VkDevice                               device,
    VkDescriptorPool                       descriptorPool,
    uint32_t                               descriptorSetCount,
    const VkDescriptorSet*                 pDescriptorSets
);

void               vkUpdateDescriptorSets
(
    VkDevice                               device,
    uint32_t                               descriptorWriteCount,
    const VkWriteDescriptorSet*            pDescriptorWrites,
    uint32_t                               descriptorCopyCount,
    const VkCopyDescriptorSet*             pDescriptorCopies
);


VkResult           vkCreateCommandPool
(
    VkDevice                               device,
    const VkCommandPoolCreateInfo*         pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkCommandPool*                         pCommandPool
);

void               vkDestroyCommandPool
(
    VkDevice                               device,
    VkCommandPool                          commandPool,
    const VkAllocationCallbacks*           pAllocator
);

VkResult           vkResetCommandPool
(
    VkDevice                               device,
    VkCommandPool                          commandPool,
    VkCommandPoolResetFlags                flags
);


void               vkGetDeviceQueue
(
    VkDevice                               device,
    uint32_t                               queueFamilyIndex,
    uint32_t                               queueIndex,
    VkQueue*                               pQueue
);


VkResult           vkQueueSubmit
(
    VkQueue                                queue,
    uint32_t                               submitCount,
    const VkSubmitInfo*                    pSubmits,
    VkFence                                fence
);

VkResult           vkQueueWaitIdle
(
    VkQueue                                queue
);

VkResult           vkQueueBindSparse
(
    VkQueue                                queue,
    uint32_t                               bindInfoCount,
    const VkBindSparseInfo*                pBindInfo,
    VkFence                                fence
);


VkResult           vkAllocateCommandBuffers
(
    VkDevice                               device,
    const VkCommandBufferAllocateInfo*     pAllocateInfo,
    VkCommandBuffer*                       pCommandBuffers
);

void               vkFreeCommandBuffers
(
    VkDevice                               device,
    VkCommandPool                          commandPool,
    uint32_t                               commandBufferCount,
    const VkCommandBuffer*                 pCommandBuffers
);


VkResult           vkBeginCommandBuffer
(
    VkCommandBuffer                        commandBuffer,
    const VkCommandBufferBeginInfo*        pBeginInfo
);

VkResult           vkEndCommandBuffer
(
    VkCommandBuffer                        commandBuffer
);

VkResult           vkResetCommandBuffer
(
    VkCommandBuffer                        commandBuffer,
    VkCommandBufferResetFlags              flags
);


void               vkCmdSetViewport
(
    VkCommandBuffer                        commandBuffer,
    uint32_t                               firstViewport,
    uint32_t                               viewportCount,
    const VkViewport*                      pViewports
);

void               vkCmdSetScissor
(
    VkCommandBuffer                        commandBuffer,
    uint32_t                               firstScissor,
    uint32_t                               scissorCount,
    const VkRect2D*                        pScissors
);

void               vkCmdSetLineWidth
(
    VkCommandBuffer                        commandBuffer,
    float                                  lineWidth
);

void               vkCmdSetDepthBias
(
    VkCommandBuffer                        commandBuffer,
    float                                  depthBiasConstantFactor,
    float                                  depthBiasClamp,
    float                                  depthBiasSlopeFactor
);

void               vkCmdSetBlendConstants
(
    VkCommandBuffer                        commandBuffer,
    const float                            blendConstants[4]
);

void               vkCmdSetDepthBounds
(
    VkCommandBuffer                        commandBuffer,
    float                                  minDepthBounds,
    float                                  maxDepthBounds
);

void               vkCmdSetStencilCompareMask
(
    VkCommandBuffer                        commandBuffer,
    VkStencilFaceFlags                     faceMask,
    uint32_t                               compareMask
);

void               vkCmdSetStencilWriteMask
(
    VkCommandBuffer                        commandBuffer,
    VkStencilFaceFlags                     faceMask,
    uint32_t                               writeMask
);

void               vkCmdSetStencilReference
(
    VkCommandBuffer                        commandBuffer,
    VkStencilFaceFlags                     faceMask,
    uint32_t                               reference
);


void               vkCmdBindPipeline
(
    VkCommandBuffer                        commandBuffer,
    VkPipelineBindPoint                    pipelineBindPoint,
    VkPipeline                             pipeline
);

void               vkCmdBindDescriptorSets
(
    VkCommandBuffer                        commandBuffer,
    VkPipelineBindPoint                    pipelineBindPoint,
    VkPipelineLayout                       layout,
    uint32_t                               firstSet,
    uint32_t                               descriptorSetCount,
    const VkDescriptorSet*                 pDescriptorSets,
    uint32_t                               dynamicOffsetCount,
    const uint32_t*                        pDynamicOffsets
);

void               vkCmdBindIndexBuffer
(
    VkCommandBuffer                        commandBuffer,
    VkBuffer                               buffer,
    VkDeviceSize                           offset,
    VkIndexType                            indexType
);

void               vkCmdBindVertexBuffers
(
    VkCommandBuffer                        commandBuffer,
    uint32_t                               firstBinding,
    uint32_t                               bindingCount,
    const VkBuffer*                        pBuffers,
    const VkDeviceSize*                    pOffsets
);


void               vkCmdDraw
(
    VkCommandBuffer                        commandBuffer,
    uint32_t                               vertexCount,
    uint32_t                               instanceCount,
    uint32_t                               firstVertex,
    uint32_t                               firstInstance
);

void               vkCmdDrawIndexed
(
    VkCommandBuffer                        commandBuffer,
    uint32_t                               indexCount,
    uint32_t                               instanceCount,
    uint32_t                               firstIndex,
    int32_t                                vertexOffset,
    uint32_t                               firstInstance
);

void               vkCmdDrawIndirect
(
    VkCommandBuffer                        commandBuffer,
    VkBuffer                               buffer,
    VkDeviceSize                           offset,
    uint32_t                               drawCount,
    uint32_t                               stride
);

void               vkCmdDrawIndexedIndirect
(
    VkCommandBuffer                        commandBuffer,
    VkBuffer                               buffer,
    VkDeviceSize                           offset,
    uint32_t                               drawCount,
    uint32_t                               stride
);


void               vkCmdDispatch
(
    VkCommandBuffer                        commandBuffer,
    uint32_t                               groupCountX,
    uint32_t                               groupCountY,
    uint32_t                               groupCountZ
);

void               vkCmdDispatchIndirect
(
    VkCommandBuffer                        commandBuffer,
    VkBuffer                               buffer,
    VkDeviceSize                           offset
);


void               vkCmdClearColorImage
(
    VkCommandBuffer                        commandBuffer,
    VkImage                                image,
    VkImageLayout                          imageLayout,
    const VkClearColorValue*               pColor,
    uint32_t                               rangeCount,
    const VkImageSubresourceRange*         pRanges
);

void               vkCmdClearDepthStencilImage
(
    VkCommandBuffer                        commandBuffer,
    VkImage                                image,
    VkImageLayout                          imageLayout,
    const VkClearDepthStencilValue*        pDepthStencil,
    uint32_t                               rangeCount,
    const VkImageSubresourceRange*         pRanges
);

void               vkCmdClearAttachments
(
    VkCommandBuffer                        commandBuffer,
    uint32_t                               attachmentCount,
    const VkClearAttachment*               pAttachments,
    uint32_t                               rectCount,
    const VkClearRect*                     pRects
);


void               vkCmdResolveImage
(
    VkCommandBuffer                        commandBuffer,
    VkImage                                srcImage,
    VkImageLayout                          srcImageLayout,
    VkImage                                dstImage,
    VkImageLayout                          dstImageLayout,
    uint32_t                               regionCount,
    const VkImageResolve*                  pRegions
);


void               vkCmdBlitImage
(
    VkCommandBuffer                        commandBuffer,
    VkImage                                srcImage,
    VkImageLayout                          srcImageLayout,
    VkImage                                dstImage,
    VkImageLayout                          dstImageLayout,
    uint32_t                               regionCount,
    const VkImageBlit*                     pRegions,
    VkFilter                               filter
);


void               vkCmdCopyImage
(
    VkCommandBuffer                        commandBuffer,
    VkImage                                srcImage,
    VkImageLayout                          srcImageLayout,
    VkImage                                dstImage,
    VkImageLayout                          dstImageLayout,
    uint32_t                               regionCount,
    const VkImageCopy*                     pRegions
);

void               vkCmdCopyBuffer
(
    VkCommandBuffer                        commandBuffer,
    VkBuffer                               srcBuffer,
    VkBuffer                               dstBuffer,
    uint32_t                               regionCount,
    const VkBufferCopy*                    pRegions
);


void               vkCmdCopyImageToBuffer
(
    VkCommandBuffer                        commandBuffer,
    VkImage                                srcImage,
    VkImageLayout                          srcImageLayout,
    VkBuffer                               dstBuffer,
    uint32_t                               regionCount,
    const VkBufferImageCopy*               pRegions
);

void               vkCmdCopyBufferToImage
(
    VkCommandBuffer                        commandBuffer,
    VkBuffer                               srcBuffer,
    VkImage                                dstImage,
    VkImageLayout                          dstImageLayout,
    uint32_t                               regionCount,
    const VkBufferImageCopy*               pRegions
);



void               vkCmdUpdateBuffer
(
    VkCommandBuffer                        commandBuffer,
    VkBuffer                               dstBuffer,
    VkDeviceSize                           dstOffset,
    VkDeviceSize                           dataSize,
    const void*                            pData
);

void               vkCmdFillBuffer
(
    VkCommandBuffer                        commandBuffer,
    VkBuffer                               dstBuffer,
    VkDeviceSize                           dstOffset,
    VkDeviceSize                           size,
    uint32_t                               data
);


void               vkCmdSetEvent
(
    VkCommandBuffer                        commandBuffer,
    VkEvent                                event,
    VkPipelineStageFlags                   stageMask
);

void               vkCmdResetEvent
(
    VkCommandBuffer                        commandBuffer,
    VkEvent                                event,
    VkPipelineStageFlags                   stageMask
);

void               vkCmdWaitEvents
(
    VkCommandBuffer                        commandBuffer,
    uint32_t                               eventCount,
    const VkEvent*                         pEvents,
    VkPipelineStageFlags                   srcStageMask,
    VkPipelineStageFlags                   dstStageMask,
    uint32_t                               memoryBarrierCount,
    const VkMemoryBarrier*                 pMemoryBarriers,
    uint32_t                               bufferMemoryBarrierCount,
    const VkBufferMemoryBarrier*           pBufferMemoryBarriers,
    uint32_t                               imageMemoryBarrierCount,
    const VkImageMemoryBarrier*            pImageMemoryBarriers
);


void               vkCmdBeginQuery
(
    VkCommandBuffer                        commandBuffer,
    VkQueryPool                            queryPool,
    uint32_t                               query,
    VkQueryControlFlags                    flags
);

void               vkCmdEndQuery
(
    VkCommandBuffer                        commandBuffer,
    VkQueryPool                            queryPool,
    uint32_t                               query
);

void               vkCmdCopyQueryPoolResults
(
    VkCommandBuffer                        commandBuffer,
    VkQueryPool                            queryPool,
    uint32_t                               firstQuery,
    uint32_t                               queryCount,
    VkBuffer                               dstBuffer,
    VkDeviceSize                           dstOffset,
    VkDeviceSize                           stride,
    VkQueryResultFlags                     flags
);

void               vkCmdResetQueryPool
(
    VkCommandBuffer                        commandBuffer,
    VkQueryPool                            queryPool,
    uint32_t                               firstQuery,
    uint32_t                               queryCount
);


void               vkCmdWriteTimestamp
(
    VkCommandBuffer                        commandBuffer,
    VkPipelineStageFlagBits                pipelineStage,
    VkQueryPool                            queryPool,
    uint32_t                               query
);


void               vkCmdPipelineBarrier
(
    VkCommandBuffer                        commandBuffer,
    VkPipelineStageFlags                   srcStageMask,
    VkPipelineStageFlags                   dstStageMask,
    VkDependencyFlags                      dependencyFlags,
    uint32_t                               memoryBarrierCount,
    const VkMemoryBarrier*                 pMemoryBarriers,
    uint32_t                               bufferMemoryBarrierCount,
    const VkBufferMemoryBarrier*           pBufferMemoryBarriers,
    uint32_t                               imageMemoryBarrierCount,
    const VkImageMemoryBarrier*            pImageMemoryBarriers
);


void               vkCmdBeginRenderPass
(
    VkCommandBuffer                        commandBuffer,
    const VkRenderPassBeginInfo*           pRenderPassBegin,
    VkSubpassContents                      contents
);

void               vkCmdNextSubpass
(
    VkCommandBuffer                        commandBuffer,
    VkSubpassContents                      contents
);

void               vkCmdEndRenderPass
(
    VkCommandBuffer                        commandBuffer
);


void               vkCmdPushConstants
(
    VkCommandBuffer                        commandBuffer,
    VkPipelineLayout                       layout,
    VkShaderStageFlags                     stageFlags,
    uint32_t                               offset,
    uint32_t                               size,
    const void*                            pValues
);


void               vkCmdExecuteCommands
(
    VkCommandBuffer                        commandBuffer,
    uint32_t                               commandBufferCount,
    const VkCommandBuffer*                 pCommandBuffers
);


// VK_KHR_xlib_surface

VkResult           vkCreateXlibSurfaceKHR
(
    VkInstance                             instance,
    const VkXlibSurfaceCreateInfoKHR*      pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkSurfaceKHR*                          pSurface
);

VkBool32           vkGetPhysicalDeviceXlibPresentationSupportKHR
(
    VkPhysicalDevice                       physicalDevice,
    uint32_t                               queueFamilyIndex,
    Display*                               dpy,
    VisualID                               visualID
);


// VK_KHR_xcb_surface

VkResult           vkCreateXcbSurfaceKHR
(
    VkInstance                             instance,
    const VkXcbSurfaceCreateInfoKHR*       pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkSurfaceKHR*                          pSurface
);

VkBool32           vkGetPhysicalDeviceXcbPresentationSupportKHR
(
    VkPhysicalDevice                       physicalDevice,
    uint32_t                               queueFamilyIndex,
    xcb_connection_t*                      connection,
    xcb_visualid_t                         visual_id
);


// VK_KHR_wayland_surface

VkResult           vkCreateWaylandSurfaceKHR
(
    VkInstance                             instance,
    const VkWaylandSurfaceCreateInfoKHR*   pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkSurfaceKHR*                          pSurface
);

VkBool32           vkGetPhysicalDeviceWaylandPresentationSupportKHR
(
    VkPhysicalDevice                       physicalDevice,
    uint32_t                               queueFamilyIndex,
    struct wl_display*                     display
);


// VK_KHR_android_surface

VkResult           vkCreateAndroidSurfaceKHR
(
    VkInstance                             instance,
    const VkAndroidSurfaceCreateInfoKHR*   pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkSurfaceKHR*                          pSurface
);


// VK_KHR_win32_surface

VkResult           vkCreateWin32SurfaceKHR
(
    VkInstance                             instance,
    const VkWin32SurfaceCreateInfoKHR*     pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkSurfaceKHR*                          pSurface
);

VkBool32           vkGetPhysicalDeviceWin32PresentationSupportKHR
(
    VkPhysicalDevice                       physicalDevice,
    uint32_t                               queueFamilyIndex
);


// VK_KHR_surface

void               vkDestroySurfaceKHR
(
    VkInstance                             instance,
    VkSurfaceKHR                           surface,
    const VkAllocationCallbacks*           pAllocator
);

VkResult           vkGetPhysicalDeviceSurfaceSupportKHR
(
    VkPhysicalDevice                       physicalDevice,
    uint32_t                               queueFamilyIndex,
    VkSurfaceKHR                           surface,
    VkBool32*                              pSupported
);

VkResult           vkGetPhysicalDeviceSurfaceCapabilitiesKHR
(
    VkPhysicalDevice                       physicalDevice,
    VkSurfaceKHR                           surface,
    VkSurfaceCapabilitiesKHR*              pSurfaceCapabilities
);

VkResult           vkGetPhysicalDeviceSurfaceFormatsKHR
(
    VkPhysicalDevice                       physicalDevice,
    VkSurfaceKHR                           surface,
    uint32_t*                              pSurfaceFormatCount,
    VkSurfaceFormatKHR*                    pSurfaceFormats);

VkResult           vkGetPhysicalDeviceSurfacePresentModesKHR
(
    VkPhysicalDevice                       physicalDevice,
    VkSurfaceKHR                           surface,
    uint32_t*                              pPresentModeCount,
    VkPresentModeKHR*                      pPresentModes
);


// VK_KHR_swapchain

VkResult           vkCreateSwapchainKHR
(
    VkDevice                               device,
    const VkSwapchainCreateInfoKHR*        pCreateInfo,
    const VkAllocationCallbacks*           pAllocator,
    VkSwapchainKHR*                        pSwapchain
);

void               vkDestroySwapchainKHR
(
    VkDevice                               device,
    VkSwapchainKHR                         swapchain,
    const VkAllocationCallbacks*           pAllocator
);

VkResult           vkGetSwapchainImagesKHR
(
    VkDevice                               device,
    VkSwapchainKHR                         swapchain,
    uint32_t*                              pSwapchainImageCount,
    VkImage*                               pSwapchainImages
);

VkResult           vkAcquireNextImageKHR
(
    VkDevice                               device,
    VkSwapchainKHR                         swapchain,
    uint64_t                               timeout,
    VkSemaphore                            semaphore,
    VkFence                                fence,
    uint32_t*                              pImageIndex
);

VkResult           vkQueuePresentKHR
(
    VkQueue                                queue,
    const VkPresentInfoKHR*                pPresentInfo
);
```
