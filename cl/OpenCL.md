
# OpenCL Objects

```C
┌────────────────────────────────────────────────┐
│                 cl_platform_id                 │
├────────────────────────────────────────────────┤
│ cl Get Platform IDs                            │
│ cl Get Platform Info                           │
└────────────────────────────────────────────────┘

┌────────────────────────────────────────────────┐
│                  cl_device_id                  │
├────────────────────────────────────────────────┤
│ cl Get        Device IDs                       │
│ cl Create Sub Devices                          │
│                                                │
│ cl Retain  Device                              │
│ cl Release Device                              │
│                                                │
│ cl Get Device Info                             │
└────────────────────────────────────────────────┘

┌────────────────────────────────────────────────┐
│                   cl_context                   │
├────────────────────────────────────────────────┤
│ cl Create Context                              │
│ cl Create Context From Type                    │
│                                                │
│ cl Retain  Context                             │
│ cl Release Context                             │
│                                                │
│ cl Get Context Info                            │
└────────────────────────────────────────────────┘

┌────────────────────────────────────────────────┐
│                cl_command_queue                │
├────────────────────────────────────────────────┤
│ cl Create CommandQueue                         │
│ cl Create CommandQueue With Properties         │
│                                                │
│ cl Retain  CommandQueue                        │
│ cl Release CommandQueue                        │
│                                                │
│ cl Get CommandQueue Info                       │
│                                                │
│ cl Flush┐                                      │
│ cl Finish                                      │
│                                                │
│ cl Enqueue Read  Buffer                        │
│ cl Enqueue Write Buffer                        │
│ cl Enqueue Fill  Buffer                        │
│ cl Enqueue Copy  Buffer                        │
│                                                │
│ cl Enqueue Read  Image                         │
│ cl Enqueue Write Image                         │
│ cl Enqueue Fill  Image                         │
│ cl Enqueue Copy  Image                         │
│                                                │
│ cl Enqueue Read  Buffer Rect                   │
│ cl Enqueue Write Buffer Rect                   │
│ cl Enqueue Copy  Buffer Rect                   │
│                                                │
│ cl Enqueue Copy Image  To Buffer               │
│ cl Enqueue Copy Buffer To Image                │
│                                                │
│ cl Enqueue Map   Buffer                        │
│ cl Enqueue Map   Image                         │
│ cl Enqueue Unmap MemObject                     │
│                                                │
│ cl Enqueue Migrate MemObjects                  │
│                                                │
│ cl Enqueue NDRange Kernel                      │
│ cl Enqueue  Native Kernel                      │
│ cl Enqueue         Task                        │
│                                                │
│ cl Enqueue Marker  With WaitList               │
│ cl Enqueue Barrier With WaitList               │
│                                                │
│ cl Enqueue SVM Free                            │
│ cl Enqueue SVM Memcpy                          │
│ cl Enqueue SVM MemFill                         │
│ cl Enqueue SVM Map                             │
│ cl Enqueue SVM Unmap                           │
│ cl Enqueue SVM MigrateMem                      │
└────────────────────────────────────────────────┘

┌────────────────────────────────────────────────┐
│                   cl_program                   │
├────────────────────────────────────────────────┤
│ cl Create Program With Source                  │
│ cl Create Program With Binary                  │
│ cl Create Program With Built In Kernels        │
│ cl Create Program With IL                      │
│                                                │
│ cl Retain  Program                             │
│ cl Release Program                             │
│                                                │
│ cl Build   Program                             │
│ cl Compile Program                             │
│ cl Link    Program                             │
│                                                │
│ cl Set Program Release Callback                │
│ cl Set Program Specialization Constant         │
│                                                │
│ cl Get Program       Info                      │
│ cl Get Program Build Info                      │
└────────────────────────────────────────────────┘

┌────────────────────────────────────────────────┐
│                   cl_kernel                    │
├────────────────────────────────────────────────┤
│ cl Create Kernel                               │
│ cl Create Kernels In Program                   │
│ cl Clone  Kernel                               │
│                                                │
│ cl Retain  Kernel                              │
│ cl Release Kernel                              │
│                                                │
│ cl Set Kernel Arg                              │
│ cl Set Kernel Arg SVMPointer                   │
│ cl Set Kernel Exec Info                        │
│                                                │
│ cl Get Kernel           Info                   │
│ cl Get Kernel       Arg Info                   │
│ cl Get Kernel WorkGroup Info                   │
│ cl Get Kernel  SubGroup Info                   │
└────────────────────────────────────────────────┘

┌────────────────────────────────────────────────┐
│                     cl_mem                     │
├────────────────────────────────────────────────┤
│ cl Create Buffer                               │
│ cl Create SubBuffer                            │
│ cl Create Image                                │
│ cl Create Pipe                                 │
│                                                │
│ cl Retain  MemObject                           │
│ cl Release MemObject                           │
│                                                │
│ cl Get MemObject Info                          │
│ cl Get Image     Info                          │
│ cl Get Pipe      Info                          │
│                                                │
│ clSetMemObjectDestructorCallback               │
└────────────────────────────────────────────────┘

┌────────────────────────────────────────────────┐
│                   cl_sampler                   │
├────────────────────────────────────────────────┤
│ cl Create Sampler                              │
│ cl Create Sampler With Properties              │
│                                                │
│ cl Retain  Sampler                             │
│ cl Release Sampler                             │
│                                                │
│ cl Get Sampler Info                            │
└────────────────────────────────────────────────┘

┌────────────────────────────────────────────────┐
│                    cl_event                    │
├────────────────────────────────────────────────┤
│ cl Create User Event                           │
│                                                │
│ cl Retain  Event                               │
│ cl Release Event                               │
│                                                │
│ cl Wait For Events                             │
│                                                │
│ cl Set User Event Status                       │
│ cl Set      Event Callback                     │
│                                                │
│ cl Get Event Info                              │
└────────────────────────────────────────────────┘
```
