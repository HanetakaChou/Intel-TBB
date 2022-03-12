#!/bin/bash

# build/index.html

ROOT_DIR="$(cd "$(dirname "$0")/../" 1>/dev/null 2>/dev/null && pwd)"

cd "${ROOT_DIR}/build"
rm -rf linux_ia32_clang_cc*_libc*_kernel*_release

cd "${ROOT_DIR}/src/"
make tbbmalloc_release compiler=clang arch=ia32 -j32 
make rml_release compiler=clang arch=ia32 -j32 
make tbb_release compiler=clang arch=ia32 -j32 