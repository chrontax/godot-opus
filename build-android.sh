#!/bin/sh

TYPE=${1:-Debug}
OUTPUT_DIR=${2:-build-android}

cmake -B "$OUTPUT_DIR" \
  -DCMAKE_BUILD_TYPE=$TYPE \
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
  -DCMAKE_POSITION_INDEPENDENT_CODE=1 \
  -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-21 \
  -DCMAKE_SYSTEM_NAME=Android \
  "${@:3}"

cmake --build $OUTPUT_DIR
