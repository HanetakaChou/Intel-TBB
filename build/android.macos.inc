# Copyright (c) 2005-2019 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

####### Detections and Commands ###############################################

# Must set def_prefix according to target architecture detected above
ifeq (ia32,$(arch))
    def_prefix = lin32
endif
ifeq (arm,$(findstring arm,$(arch)))
    def_prefix = lin32
endif
ifeq (64,$(findstring 64,$(arch)))
    def_prefix = lin64
endif

ifdef ndk_version
    $(warning "NDK version $(ndk_version)")
else
    $(warning "NDK version not set in environment, using \'unknown\' instead.")
    ndk_version:=unknown
endif

export runtime:=$(target)_NDK$(ndk_version)_version_$(target_os_version)

AR = $(tbb_tool_prefix)ar
MAKE_VERSIONS=sh $(tbb_root)/build/version_info_android.sh $(VERSION_FLAGS) >version_string.ver

####### Build settings ########################################################

# No SONAME_SUFFIX for Android allowed in library names
TBB.LST = $(tbb_root)/src/tbb/$(def_prefix)-tbb-export.lst
TBB.DEF = $(TBB.LST:.lst=.def)
TBB.DLL = libpt_tbb$(CPF_SUFFIX).$(DLL)
TBB.LIB = $(TBB.DLL)
TBB_NO_VERSION.DLL=
LINK_TBB.LIB = $(TBB.LIB)

MALLOC.DEF = $(MALLOC_ROOT)/$(def_prefix)-tbbmalloc-export.def
MALLOC.DLL = libpt_tbbmalloc.$(DLL)
MALLOC.LIB = $(MALLOC.DLL)
MALLOC_NO_VERSION.DLL=
LINK_MALLOC.LIB = $(MALLOC.LIB)

MALLOCPROXY.DEF = $(MALLOC_ROOT)/$(def_prefix)-proxy-export.def
MALLOCPROXY.DLL = ibpt_tbbmalloc_proxy.$(DLL)
MALLOCPROXY_NO_VERSION.DLL=
MALLOCPROXY.LIB = $(MALLOCPROXY.DLL)
LINK_MALLOCPROXY.LIB = $(MALLOCPROXY.LIB)

RML.DEF = $(RML_SERVER_ROOT)/$(def_prefix)-rml-export.def
RML.DLL = libpt_irml.$(DLL)
RML.LIB = $(RML.DLL)
LINK_RML.LIB = $(RML.LIB)

TBB.RES =
MALLOC.RES =
RML.RES =
TBB.MANIFEST =
MALLOC.MANIFEST =
RML.MANIFEST =
OBJ = o
DLL = so

TEST_LAUNCHER=
run_cmd ?= -sh $(tbb_root)/build/android.linux.launcher.sh $(largs)
