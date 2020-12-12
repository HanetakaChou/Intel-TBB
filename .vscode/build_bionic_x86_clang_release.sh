#!/bin/bash

# build/index.html

ROOT_DIR="$(cd "$(dirname "$0")/../" 1>/dev/null 2>/dev/null && pwd)"

cd "${ROOT_DIR}/build"
rm -rf linux_ia32_clang_android*_NDK*_version*_release

cd "${ROOT_DIR}/src/"
NDK_DIR="${ROOT_DIR}/build/android-ndk-r14b"
"${NDK_DIR}/ndk-build" tbbmalloc_release compiler=clang arch=ia32 -j32 
"${NDK_DIR}/ndk-build" rml_release compiler=clang arch=ia32 -j32 
"${NDK_DIR}/ndk-build" tbb_release compiler=clang arch=ia32 -j32 