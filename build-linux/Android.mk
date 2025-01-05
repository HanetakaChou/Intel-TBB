#
# Copyright (C) YuqiaoZhang(HanetakaChou)
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

# https://developer.android.com/ndk/guides/android_mk

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Intel-TBB

LOCAL_SRC_FILES := \
    $(LOCAL_PATH)/../src/old/concurrent_queue_v2.cpp \
    $(LOCAL_PATH)/../src/old/concurrent_vector_v2.cpp \
    $(LOCAL_PATH)/../src/old/spin_rw_mutex_v2.cpp \
    $(LOCAL_PATH)/../src/old/task_v2.cpp \
    $(LOCAL_PATH)/../src/rml/client/rml_tbb.cpp \
    $(LOCAL_PATH)/../src/rml/server/rml_server.cpp \
    $(LOCAL_PATH)/../src/tbbmalloc/backend.cpp \
    $(LOCAL_PATH)/../src/tbbmalloc/backref.cpp \
    $(LOCAL_PATH)/../src/tbbmalloc/frontend.cpp \
    $(LOCAL_PATH)/../src/tbbmalloc/large_objects.cpp \
    $(LOCAL_PATH)/../src/tbbmalloc/tbbmalloc.cpp \
    $(LOCAL_PATH)/../src/tbb/arena.cpp \
    $(LOCAL_PATH)/../src/tbb/cache_aligned_allocator.cpp \
    $(LOCAL_PATH)/../src/tbb/concurrent_hash_map.cpp \
    $(LOCAL_PATH)/../src/tbb/concurrent_monitor.cpp \
    $(LOCAL_PATH)/../src/tbb/concurrent_queue.cpp \
    $(LOCAL_PATH)/../src/tbb/concurrent_vector.cpp \
    $(LOCAL_PATH)/../src/tbb/condition_variable.cpp \
    $(LOCAL_PATH)/../src/tbb/critical_section.cpp \
    $(LOCAL_PATH)/../src/tbb/governor.cpp \
    $(LOCAL_PATH)/../src/tbb/itt_notify.cpp \
    $(LOCAL_PATH)/../src/tbb/market.cpp \
    $(LOCAL_PATH)/../src/tbb/mutex.cpp \
    $(LOCAL_PATH)/../src/tbb/observer_proxy.cpp \
    $(LOCAL_PATH)/../src/tbb/pipeline.cpp \
    $(LOCAL_PATH)/../src/tbb/private_server.cpp \
    $(LOCAL_PATH)/../src/tbb/queuing_mutex.cpp \
    $(LOCAL_PATH)/../src/tbb/queuing_rw_mutex.cpp \
    $(LOCAL_PATH)/../src/tbb/reader_writer_lock.cpp \
    $(LOCAL_PATH)/../src/tbb/recursive_mutex.cpp \
    $(LOCAL_PATH)/../src/tbb/scheduler.cpp \
    $(LOCAL_PATH)/../src/tbb/semaphore.cpp \
    $(LOCAL_PATH)/../src/tbb/spin_mutex.cpp \
    $(LOCAL_PATH)/../src/tbb/spin_rw_mutex.cpp \
    $(LOCAL_PATH)/../src/tbb/task.cpp \
    $(LOCAL_PATH)/../src/tbb/task_group_context.cpp \
    $(LOCAL_PATH)/../src/tbb/tbb_main.cpp \
    $(LOCAL_PATH)/../src/tbb/tbb_misc.cpp \
    $(LOCAL_PATH)/../src/tbb/tbb_misc_ex.cpp \
    $(LOCAL_PATH)/../src/tbb/tbb_statistics.cpp \
    $(LOCAL_PATH)/../src/tbb/tbb_thread.cpp \
    $(LOCAL_PATH)/../src/tbb/x86_rtm_rw_mutex.cpp

LOCAL_CFLAGS :=

ifeq (armeabi-v7a,$(TARGET_ARCH_ABI))
LOCAL_ARM_MODE := arm
LOCAL_ARM_NEON := true
else ifeq (arm64-v8a,$(TARGET_ARCH_ABI))
LOCAL_CFLAGS +=
else ifeq (x86,$(TARGET_ARCH_ABI))
LOCAL_CFLAGS += -mf16c
LOCAL_CFLAGS += -mfma
LOCAL_CFLAGS += -mavx2
else ifeq (x86_64,$(TARGET_ARCH_ABI))
LOCAL_CFLAGS += -mf16c
LOCAL_CFLAGS += -mfma
LOCAL_CFLAGS += -mavx2
else
LOCAL_CFLAGS +=
endif

LOCAL_CFLAGS += -Wall
LOCAL_CFLAGS += -Werror=return-type

LOCAL_CFLAGS += -DTBB_SUPPRESS_DEPRECATED_MESSAGES=1
LOCAL_CFLAGS += -D__TBB_BUILD=1
LOCAL_CFLAGS += -D__TBBMALLOC_BUILD=1
LOCAL_CFLAGS += -D__TBB_NO_IMPLICIT_LINKAGE=1
LOCAL_CFLAGS += -D__TBBMALLOC_NO_IMPLICIT_LINKAGE=1
LOCAL_CFLAGS += -DUSE_PTHREAD

LOCAL_C_INCLUDES :=
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../src/rml/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../build/vs2013

LOCAL_CPPFLAGS := 
LOCAL_CPPFLAGS += -std=c++17

include $(BUILD_STATIC_LIBRARY)
