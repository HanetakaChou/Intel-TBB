#!/bin/bash

# build/index.html

ROOT_DIR="$(cd "$(dirname "$0")/../" 1>/dev/null 2>/dev/null && pwd)"  

cd "${ROOT_DIR}/build"
rm -rf macos_intel64_clang_cc*_os*_release
rm -rf macos_arm64_clang_cc*_os*_release

cd "${ROOT_DIR}/src/"
make target=macos tbbmalloc_release arch=intel64 -j32 
make target=macos rml_release arch=intel64 -j32
make target=macos tbb_release arch=intel64 -j32

make target=macos tbbmalloc_release arch=arm64 -j32 
make target=macos rml_release arch=arm64 -j32
make target=macos tbb_release arch=arm64 -j32

