
#
# BSD 3-Clause License
#
# Copyright (c) 2018, mtezych
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

Ninja ()
{
    mkdir -p $TOOLCHAIN_ROOT/ninja/bin
    cd       $TOOLCHAIN_ROOT/ninja/bin
    wget     https://github.com/ninja-build/ninja/releases/download/v1.8.2/ninja-linux.zip
    unzip    ninja-linux.zip
    rm       ninja-linux.zip
}

GNU_Make ()
{
    mkdir -p $TOOLCHAIN_ROOT/make/bin
    cd       $TOOLCHAIN_ROOT/make/bin
    cp       /usr/bin/make make # This needs to be fixed!
}

CMake ()
{
    cd   $TOOLCHAIN_ROOT
    wget https://github.com/Kitware/CMake/releases/download/v3.13.2/cmake-3.13.2-Linux-x86_64.tar.gz
    tar -xvf cmake-3.13.2-Linux-x86_64.tar.gz
    rm       cmake-3.13.2-Linux-x86_64.tar.gz
    mv       cmake-3.13.2-Linux-x86_64 cmake
}

LLVM_Clang ()
{
    # LLVM   -> https://git.llvm.org/git/llvm.git
    cd          $TOOLCHAIN_ROOT
    git clone   https://github.com/mtezych/llvm.git llvm-project

    # Clang  -> https://git.llvm.org/git/clang.git
    #           https://git.llvm.org/git/clang-tools-extra.git
    cd          $TOOLCHAIN_ROOT/llvm-project/tools
    git clone   https://github.com/mtezych/clang.git

    cd          $TOOLCHAIN_ROOT/llvm-project/tools/clang/tools
    git clone   https://github.com/mtezych/clang-tools-extra.git extra

    # LLD    -> https://git.llvm.org/git/lld.git
    cd          $TOOLCHAIN_ROOT/llvm-project/tools
    git clone   https://github.com/mtezych/lld.git

    # libc++ -> https://git.llvm.org/git/libcxx.git
    #           https://git.llvm.org/git/libcxxabi.git
    cd          $TOOLCHAIN_ROOT/llvm-project/projects
    git clone   https://github.com/mtezych/libcxx.git
    git clone   https://github.com/mtezych/libcxxabi.git

    # Build
    mkdir -p $TOOLCHAIN_ROOT/llvm-build
    cd       $TOOLCHAIN_ROOT/llvm-build
    cmake -DCMAKE_BUILD_TYPE=Release                  \
          -DCMAKE_INSTALL_PREFIX=$TOOLCHAIN_ROOT/llvm \
          -DLLVM_ENABLE_ASSERTIONS=OFF                \
          -G Ninja $TOOLCHAIN_ROOT/llvm-project
    ninja
    ninja install
}

TOOLCHAIN_ROOT=$PWD

Ninja
GNU_Make
CMake
LLVM_Clang
