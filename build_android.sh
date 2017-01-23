#!/bin/bash

########################################################
#     DO NOT RUN THIS SCRIPT WITHOUT THINKING !!!      #
#                                                      #
# This script should be thought as an example of steps #
# required to build binaries for the Android OS.       #
########################################################

# Android source code
# https://source.android.com/source/downloading.html
# https://source.android.com/source/using-repo.html
mkdir android-source
cd android-source
repo init -u https://android.googlesource.com/platform/manifest # upstream
repo sync system/core          \
          hardware/libhardware \
          frameworks/native
cd ..

# Android sysroot
# https://developer.android.com/studio/command-line/adb.html
mkdir -p android-sysroot/system/lib
adb pull /system/lib/libgui.so   android-sysroot/system/lib/libgui.so
adb pull /system/lib/libutils.so android-sysroot/system/lib/libutils.so

# Android NDK
# https://developer.android.com/ndk/index.html
wget https://dl.google.com/android/repository/android-ndk-r13b-linux-x86_64.zip
unzip android-ndk-r13b-linux-x86_64.zip
rm android-ndk-r13b-linux-x86_64.zip

# Android standalone toolchain
# https://developer.android.com/ndk/guides/standalone_toolchain.html
cd android-ndk-r13b/build/tools
python make_standalone_toolchain.py \
--arch arm                          \
--api 21                            \
--install-dir `pwd`/../../../android-standalone-toolchain
cd ../../..

# CMake 3.7.2
wget https://cmake.org/files/v3.7/cmake-3.7.2-Linux-x86_64.tar.gz
tar -xvf cmake-3.7.2-Linux-x86_64.tar.gz
rm cmake-3.7.2-Linux-x86_64.tar.gz

# CMake & Make
# https://cmake.org/cmake/help/v3.7/manual/cmake-toolchains.7.html
mkdir build
cd build
if [ $# -eq 0 ] # any arguments specified
then
    ../cmake-3.7.2-Linux-x86_64/bin/cmake ..      \
    -DCMAKE_SYSTEM_NAME=Android                   \
    -DCMAKE_ANDROID_API=21                        \
    -DCMAKE_ANDROID_ARCH_ABI=armeabi-v7a          \
    -DCMAKE_ANDROID_NDK=`pwd`/../android-ndk-r13b \
    -DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang   \
    -DCMAKE_ANDROID_STL_TYPE=gnustl_shared        \
    -DANDROID_SOURCE=`pwd`/../android-source      \
    -DANDROID_SYSROOT=`pwd`/../android-sysroot
else
    ../../cmake-3.7.2-Linux-x86_64/bin/cmake ..                                \
    -DCMAKE_SYSTEM_NAME=Android                                                \
    -DCMAKE_ANDROID_STANDALONE_TOOLCHAIN=`pwd`/../android-standalone-toolchain \
    -DANDROID_SOURCE=`pwd`/../android-source                                   \
    -DANDROID_SYSROOT=`pwd`/../android-sysroot
fi
make
cd ..
