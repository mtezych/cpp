
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

cmake_minimum_required(VERSION 3.1)

add_library               (drm SHARED xf86drm.h
                                      xf86drm.c
                                      xf86drmHash.h
                                      xf86drmHash.c
                                      xf86drmRandom.h
                                      xf86drmRandom.c
                                      xf86drmMode.h
                                      xf86drmMode.c
                                      xf86drmSL.c
                                      xf86atomic.h
                                      libdrm_lists.h
                                      libdrm_macros.h
                                      util_double_list.h
                                      util_math.h
                                      libsync.h
                                      config.h
                                      include/drm/drm_fourcc.h
                                      include/drm/drm.h
                                      include/drm/drm_mode.h
                                      include/drm/drm_sarea.h
                                      include/drm/i915_drm.h
                                      include/drm/mach64_drm.h
                                      include/drm/mga_drm.h
                                      include/drm/qxl_drm.h
                                      include/drm/virtgpu_drm.h)

target_include_directories(drm PUBLIC . include/drm /usr/include/valgrind)

target_compile_definitions(drm PUBLIC PIC HAVE_CONFIG_H)

target_link_libraries     (drm m)
