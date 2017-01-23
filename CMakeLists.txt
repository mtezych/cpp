
cmake_minimum_required (VERSION 3.1)
project(cpp)

if(ANDROID_SOURCE AND ANDROID_SYSROOT)
    add_executable            (android-window android-window.cpp)
    target_include_directories(android-window PUBLIC ${ANDROID_SOURCE}/frameworks/native/include
                                                     ${ANDROID_SOURCE}/system/core/include
                                                     ${ANDROID_SOURCE}/hardware/libhardware/include)
    target_link_libraries     (android-window ${ANDROID_SYSROOT}/system/lib/libutils.so
                                              ${ANDROID_SYSROOT}/system/lib/libgui.so)
    set_target_properties     (android-window PROPERTIES CXX_STANDARD          14
                                                         CXX_STANDARD_REQUIRED ON)
else()
    message("Android source code and sysroot are required.")
endif()

add_executable       (list list.cpp)
set_target_properties(list PROPERTIES CXX_STANDARD          14
                                      CXX_STANDARD_REQUIRED ON)

add_executable       (ieee754-float ieee754-float.cpp)
set_target_properties(ieee754-float PROPERTIES CXX_STANDARD          14
                                               CXX_STANDARD_REQUIRED ON)

# http://en.cppreference.com/w/cpp/language/constraints
if((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") AND (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "6.1"))
           add_executable (concepts-lite concepts-lite.cpp)
     set_target_properties(concepts-lite PROPERTIES CXX_STANDARD          14
                                                    CXX_STANDARD_REQUIRED ON)
    target_compile_options(concepts-lite PUBLIC -fconcepts)
else()
    message("C++ concepts lite require GCC 6.1 or later.")
endif()
