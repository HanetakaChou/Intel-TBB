#!/bin/bash

ROOT_DIR="$(cd "$(dirname "$0")/../" 1>/dev/null 2>/dev/null && pwd)"  

mkdir -p "${ROOT_DIR}/build/ios_univeral_release"
OUT_BINS=("libpt_tbb.a" "libpt_irml.a" "libpt_tbbmalloc.a")
for i in "${OUT_BINS[@]}"
do
    libtool -static "${ROOT_DIR}/build/macos_intel64_clang_cc12.0.5_ios14.5_release/${i}" "${ROOT_DIR}/build/macos_arm64_clang_cc12.0.5_ios14.5_release/${i}" -o "${ROOT_DIR}/build/ios_univeral_release/${i}"
done