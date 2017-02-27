
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

add_executable            (vulkan VulkanSymbols.h
                                  Vulkan.h
                                  Vulkan.cpp
                                  VulkanInstance.h
                                  VulkanInstance.cpp
                                  VulkanSurface.h
                                  VulkanSurface.cpp
                                  VulkanPhysicalDevice.h
                                  VulkanPhysicalDevice.cpp
                                  VulkanDevice.h
                                  VulkanDevice.cpp
                                  VulkanQueue.h
                                  VulkanQueue.cpp
                                  VulkanSemaphore.h
                                  VulkanSemaphore.cpp
                                  VulkanCommandPool.h
                                  VulkanCommandPool.cpp
                                  VulkanCommandBuffer.h
                                  VulkanCommandBuffer.cpp
                                  VulkanSwapchain.h
                                  VulkanSwapchain.cpp
                                  vulkan-demo.cpp)

target_include_directories(vulkan PUBLIC $ENV{VULKAN_SDK}/include)

target_link_libraries     (vulkan library
                                  ${CMAKE_DL_LIBS})

set_target_properties     (vulkan PROPERTIES CXX_STANDARD          14
                                             CXX_STANDARD_REQUIRED ON)
