
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

project(platform-android)

if(CMAKE_SYSTEM_NAME MATCHES Android)

 if(ANDROID_SOURCE AND ANDROID_SYSROOT)

  add_library               (platform-android STATIC include/android/Window.h
                                                     include/android/Pixmap.h
                                                              source/Window.cpp
                                                              source/Pixmap.cpp)

  target_include_directories(platform-android PUBLIC include
                                 ${ANDROID_SOURCE}/frameworks/native/include
                                 ${ANDROID_SOURCE}/system/core/include
                                 ${ANDROID_SOURCE}/hardware/libhardware/include)

  target_link_libraries     (platform-android util
                                 ${ANDROID_SYSROOT}/system/lib/libui.so
                                 ${ANDROID_SYSROOT}/system/lib/libgui.so
                                 ${ANDROID_SYSROOT}/system/lib/libutils.so)

  set_target_properties     (platform-android PROPERTIES CXX_STANDARD          14
                                                         CXX_STANDARD_REQUIRED ON)

 else()

  message("[platform-android] Android source code and sysroot are required.")

 endif()


 add_executable             (android-demo demo/android-demo.cpp)

 target_link_libraries      (android-demo platform-android)

 set_target_properties      (android-demo PROPERTIES CXX_STANDARD          14
                                                     CXX_STANDARD_REQUIRED ON)

else()

 message("[platform-android] Only building for Android is supported.")

endif()
