#!/bin/bash

# build/index.html

ROOT_DIR="$(cd "$(dirname "$0")/../" 1>/dev/null 2>/dev/null && pwd)"

cd "${ROOT_DIR}/src/"
make tbb_test_debug_no_depends compiler=gcc arch=intel64 -j32
make examples_debug_no_depends compiler=gcc arch=intel64 -j32