#!/bin/bash

# build/index.html

ROOT_DIR="$(cd "$(dirname "$0")/../" 1>/dev/null 2>/dev/null && pwd)"  

cd "${ROOT_DIR}/build"
rm -rf macos_intel64_clang_cc*_os*_debug

cd "${ROOT_DIR}/src/"
make tbbmalloc_debug -j32 
make rml_debug -j32
make tbb_debug -j32