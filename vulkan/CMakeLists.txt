
#
# BSD 3-Clause License
#
# Copyright (c) 2017, mtezych
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

cmake_minimum_required (VERSION 3.1)

project(vulkan)

add_library               (vulkan STATIC include/vulkan/Symbols.h
                                         include/vulkan/Version.h
                                         include/vulkan/Loader.h
                                         include/vulkan/Instance.h
                                         include/vulkan/Surface.h
                                         include/vulkan/PhysicalDevice.h
                                         include/vulkan/Device.h
                                         include/vulkan/Queue.h
                                         include/vulkan/Semaphore.h
                                         include/vulkan/CommandPool.h
                                         include/vulkan/CommandBuffer.h
                                         include/vulkan/Swapchain.h
                                         include/vulkan/Pipeline.h
                                         include/vulkan/PipelineCache.h
                                         include/vulkan/PipelineLayout.h
                                         include/vulkan/Framebuffer.h
                                         include/vulkan/RenderPass.h
                                         include/vulkan/Image.h
                                         include/vulkan/ImageView.h
                                         include/vulkan/ShaderModule.h
                                         include/vulkan/DeviceMemory.h
                                         include/vulkan/Buffer.h
                                         include/vulkan/BufferView.h
                                         include/vulkan/Fence.h
                                         include/vulkan/DescriptorPool.h
                                         include/vulkan/DescriptorSetLayout.h
                                         include/vulkan/Sampler.h
                                                 source/Loader.cpp
                                                 source/Instance.cpp
                                                 source/Surface.cpp
                                                 source/PhysicalDevice.cpp
                                                 source/Device.cpp
                                                 source/Queue.cpp
                                                 source/Semaphore.cpp
                                                 source/CommandPool.cpp
                                                 source/CommandBuffer.cpp
                                                 source/Swapchain.cpp
                                                 source/Pipeline.cpp
                                                 source/PipelineCache.cpp
                                                 source/PipelineLayout.cpp
                                                 source/Framebuffer.cpp
                                                 source/RenderPass.cpp
                                                 source/Image.cpp
                                                 source/ImageView.cpp
                                                 source/ShaderModule.cpp
                                                 source/DeviceMemory.cpp
                                                 source/Buffer.cpp
                                                 source/BufferView.cpp
                                                 source/Fence.cpp
                                                 source/DescriptorPool.cpp
                                                 source/DescriptorSetLayout.cpp
                                                 source/Sampler.cpp)

target_include_directories(vulkan PUBLIC include)

target_compile_definitions(vulkan PUBLIC VK_NO_PROTOTYPES)

target_link_libraries     (vulkan khronos platform)

if     (CMAKE_SYSTEM_NAME MATCHES Linux)

target_compile_definitions(vulkan PUBLIC VK_USE_PLATFORM_XLIB_KHR)
target_link_libraries     (vulkan platform-xlib)

elseif (CMAKE_SYSTEM_NAME MATCHES Android)

target_compile_definitions(vulkan PUBLIC VK_USE_PLATFORM_ANDROID_KHR)
target_link_libraries     (vulkan platform-android)

elseif (CMAKE_SYSTEM_NAME MATCHES Windows)

target_compile_definitions(vulkan PUBLIC VK_USE_PLATFORM_WIN32_KHR)
target_link_libraries     (vulkan platform-windows)

else ()

message("[vulkan] Only building for GNU/Linux, Android & Windows is supported.")

endif ()

set_target_properties     (vulkan PROPERTIES CXX_STANDARD          14
                                             CXX_STANDARD_REQUIRED ON)


add_executable            (vulkan-demo demo/vulkan-demo.cpp)

target_link_libraries     (vulkan-demo vulkan)

set_target_properties     (vulkan-demo PROPERTIES CXX_STANDARD          14
                                                  CXX_STANDARD_REQUIRED ON)
