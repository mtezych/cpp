
cmake_minimum_required(VERSION 3.1)

project(libdrm-example)

add_subdirectory(libdrm)
add_subdirectory(libdrm/libkms)
add_subdirectory(libdrm/intel)

add_executable       (libdrm-example libdrm-example.cpp)

target_link_libraries(libdrm-example drm kms drm_intel)

set_target_properties(libdrm-example PROPERTIES CXX_STANDARD          14
                                                CXX_STANDARD_REQUIRED ON)
