#!/bin/bash  

ROOT_DIR="$(cd "$(dirname "$0")/../" 1>/dev/null 2>/dev/null && pwd)"

cd "${ROOT_DIR}/build"

rm -rf android-ndk-r14b-linux-x86_64.zip
if curl https://dl.google.com/android/repository/android-ndk-r14b-linux-x86_64.zip -o android-ndk-r14b-linux-x86_64.zip; then
    echo "curl android-ndk-r14b-linux-x86_64.zip passed"
else
    echo "curl android-ndk-r14b-linux-x86_64.zip failed"
    exit 1
fi

rm -rf android-ndk-r14b
if unzip android-ndk-r14b-linux-x86_64.zip; then 
    echo "unzip android-ndk-r14b-linux-x86_64.zip passed"
else
    echo "unzip android-ndk-r14b-linux-x86_64.zip failed"
    exit 1
fi
rm -rf android-ndk-r14b-linux-x86_64.zip
