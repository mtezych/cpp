
cmake_minimum_required (VERSION 3.11)

project(StackAlloc VERSION 0.1.0 LANGUAGES CXX)


add_executable         (stack-alloc)

target_sources         (stack-alloc PRIVATE stack-alloc.cxx)

target_compile_features(stack-alloc PRIVATE cxx_std_17)
