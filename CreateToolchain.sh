#!/bin/bash

#
# BSD 3-Clause License
#
# Copyright (c) 2019, mtezych
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

#
# Linux Kernel
# https://www.kernel.org
#
# GNU Binary Utilities
# https://sourceware.org/binutils/docs-2.32/binutils/index.html
#
# GNU Compiler Collection
# https://gcc.gnu.org/install/configure.html
#
# GNU C Library
# https://www.gnu.org/software/libc/manual/html_node/Configuring-and-compiling.html
#

PrepareHost ()
{
    sudo apt update
    sudo apt install wget tar        \
                     gcc g++         \
                     build-essential \
                     make texinfo    \
                     autoconf        \
                     bison flex      \
                     gawk
}

PrepareSources ()
{
    local LINUX=linux-$LINUX_VER
    local BINUTILS=binutils-$BINUTILS_VER
    local GLIBC=glibc-$GLIBC_VER
    local GCC=gcc-$GCC_VER

    cd $TOOLCHAIN_ROOT

    wget https://www.kernel.org/pub/linux/kernel/v4.x/$LINUX.tar.xz
    wget https://ftp.gnu.org/gnu/binutils/$BINUTILS.tar.xz
    wget https://ftp.gnu.org/gnu/glibc/$GLIBC.tar.xz
    wget https://ftp.gnu.org/gnu/gcc/$GCC/$GCC.tar.xz

    tar -xvf    $LINUX.tar.xz
    tar -xvf $BINUTILS.tar.xz
    tar -xvf    $GLIBC.tar.xz
    tar -xvf      $GCC.tar.xz

    mv $LINUX       linux-src
    mv $BINUTILS binutils-src
    mv $GLIBC       glibc-src
    mv $GCC           gcc-src

    cd gcc-src
    $TOOLCHAIN_ROOT/gcc-src/contrib/download_prerequisites
}

BuildBinutils ()
{
    mkdir $TOOLCHAIN_ROOT/binutils-build
    cd    $TOOLCHAIN_ROOT/binutils-build
    $TOOLCHAIN_ROOT/binutils-src/configure                                     \
                                    --prefix=$TOOLCHAIN_ROOT/gcc               \
                                    --build=$HOST                              \
                                    --host=$HOST                               \
                                    --target=$TARGET                           \
                                    --with-sysroot=$TOOLCHAIN_ROOT/gcc/sysroot
    make all -j4
    make install
}

BuildLinuxHeaders ()
{
    cd $TOOLCHAIN_ROOT/linux-src
    make headers_install                                  \
         ARCH=$ARCH                                       \
         INSTALL_HDR_PATH=$TOOLCHAIN_ROOT/gcc/sysroot/usr
}

BuildFirstGCC ()
{
    mkdir $TOOLCHAIN_ROOT/gcc-first-build
    cd    $TOOLCHAIN_ROOT/gcc-first-build
    $TOOLCHAIN_ROOT/gcc-src/configure                                          \
                                    --prefix=$TOOLCHAIN_ROOT/gcc               \
                                    --build=$HOST                              \
                                    --host=$HOST                               \
                                    --target=$TARGET                           \
                                    --with-sysroot=$TOOLCHAIN_ROOT/gcc/sysroot \
                                    --with-multilib-list=$MULTILIB             \
                                    --enable-languages=c                       \
                                    --with-glibc-version=$GLIBC_VER            \
                                    --with-newlib                              \
                                    --without-headers                          \
                                    --disable-shared                           \
                                    --disable-threads                          \
                                    --disable-libssp                           \
                                    --disable-libatomic                        \
                                    --disable-libgomp                          \
                                    --disable-libmpx                           \
                                    --disable-libquadmath
    make all -j4
    make install
}

BuildGlibc ()
{
    mkdir $TOOLCHAIN_ROOT/glibc-build
    cd    $TOOLCHAIN_ROOT/glibc-build
    $TOOLCHAIN_ROOT/glibc-src/configure                                        \
                                    --prefix=/usr                              \
                                    --build=$HOST                              \
                                    --host=$TARGET                             \
                                    --enable-profile
    make all -j4
    make install install_root=$TOOLCHAIN_ROOT/gcc/sysroot
}

BuildGCC ()
{
    mkdir -p $TOOLCHAIN_ROOT/gcc/sysroot/lib
    mkdir -p $TOOLCHAIN_ROOT/gcc/sysroot/usr/lib
    mkdir $TOOLCHAIN_ROOT/gcc-build
    cd    $TOOLCHAIN_ROOT/gcc-build
    $TOOLCHAIN_ROOT/gcc-src/configure                                          \
                                    --prefix=$TOOLCHAIN_ROOT/gcc               \
                                    --build=$HOST                              \
                                    --host=$HOST                               \
                                    --target=$TARGET                           \
                                    --with-sysroot=$TOOLCHAIN_ROOT/gcc/sysroot \
                                    --with-multilib-list=$MULTILIB             \
                                    --enable-languages=c,c++                   \
                                    --enable-shared                            \
                                    --enable-threads
    make all -j4
    make install
}

main ()
{
    TOOLCHAIN_ROOT=$PWD

    LINUX_VER=4.19
    BINUTILS_VER=2.31
    GLIBC_VER=2.28
    GCC_VER=8.2.0

    ARCH=x86
    MULTILIB=m64
    HOST=x86_64-unknown-linux-gnu
    TARGET=x86_64-pc-linux-gnu

    PrepareHost
    PrepareSources

    export PATH=$PATH:$TOOLCHAIN_ROOT/gcc/bin

    BuildBinutils
    BuildLinuxHeaders
    BuildFirstGCC
    BuildGlibc
    BuildGCC
}

set -xe
main
