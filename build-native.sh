#!/bin/sh

TYPE=${1:-Debug}
OUTPUT_DIR=${2:-build}

cmake -B "$OUTPUT_DIR" \
  -DCMAKE_BUILD_TYPE=$TYPE \
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
  "${@:3}"

cmake --build $OUTPUT_DIR
