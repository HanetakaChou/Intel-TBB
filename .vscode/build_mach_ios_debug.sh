#!/bin/bash

# build/index.html

ROOT_DIR="$(cd "$(dirname "$0")/../" 1>/dev/null 2>/dev/null && pwd)"  

cd "${ROOT_DIR}/build"
rm -rf macos_arm64_clang_cc*_ios*_debug

cd "${ROOT_DIR}/src/"
make target=ios tbbmalloc_debug arch=arm64 -j32 
make target=ios rml_debug arch=arm64 -j32
make target=ios tbb_debug arch=arm64 -j32
 