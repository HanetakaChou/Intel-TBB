#!/bin/bash

# build/index.html

ROOT_DIR="$(cd "$(dirname "$0")/../" 1>/dev/null 2>/dev/null && pwd)"

cd "${ROOT_DIR}/build"
rm -rf linux_ia32_clang_cc*_libc*_kernel*_debug

cd "${ROOT_DIR}/src/"
make tbbmalloc_debug compiler=clang arch=ia32 -j32 
make rml_debug compiler=clang arch=ia32 -j32 
make tbb_debug compiler=clang arch=ia32 -j32 