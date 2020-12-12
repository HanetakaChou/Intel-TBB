#!/bin/bash

ROOT_DIR="$(cd "$(dirname "$0")/../" 1>/dev/null 2>/dev/null && pwd)"  

mkdir -p "${ROOT_DIR}/build/macos_univeral_release"
OUT_BINS=("libpt_tbb.dylib" "libpt_irml.dylib" "libpt_tbbmalloc.dylib")
for i in "${OUT_BINS[@]}"
do
    lipo "${ROOT_DIR}/build/macos_arm64_clang_cc12.0.5_os11.4_release/${i}" "${ROOT_DIR}/build/macos_intel64_clang_cc12.0.5_os11.4_release/${i}" -create -output "${ROOT_DIR}/build/macos_univeral_release/${i}"
done