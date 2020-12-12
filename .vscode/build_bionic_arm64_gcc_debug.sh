#!/bin/bash

# build/index.html

ROOT_DIR="$(cd "$(dirname "$0")/../" 1>/dev/null 2>/dev/null && pwd)"

cd "${ROOT_DIR}/build"
rm -rf linux_arm64_gcc_android*_NDK*_version*_debug

cd "${ROOT_DIR}/src/"
NDK_DIR="${ROOT_DIR}/build/android-ndk-r14b"
"${NDK_DIR}/ndk-build" target=android tbbmalloc_debug compiler=gcc arch=arm64 -j32 
"${NDK_DIR}/ndk-build" target=android rml_debug compiler=gcc arch=arm64 -j32 
"${NDK_DIR}/ndk-build" target=android tbb_debug compiler=gcc arch=arm64 -j32 