#!/bin/bash

ROOT_DIR="$(cd "$(dirname "$0")/../" 1>/dev/null 2>/dev/null && pwd)"  

mkdir -p "${ROOT_DIR}/build/ios_univeral_release"
OUT_BINS=("libpt_tbb" "libpt_irml" "libpt_tbbmalloc")
for i in "${OUT_BINS[@]}"
do
    libtool -static "${ROOT_DIR}/build/macos_intel64_clang_cc13.0.0_ios15.2_release/${i}.a" "${ROOT_DIR}/build/macos_arm64_clang_cc13.0.0_ios15.2_release/${i}.a" -o "${ROOT_DIR}/build/ios_univeral_release/${i}_catalyst.a"
done