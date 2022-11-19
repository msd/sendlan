#!/usr/bin/sh

BASE_DIR=$(realpath $(dirname "$0"))
SRC_DIR="$BASE_DIR/src"
BUILD_DIR="$BASE_DIR/build"
CONAN_PROFILE=""

function using_conan {
    return $( [[ -f "$SRC_DIR/conanfile.txt" ]] )
}

rm -rf "$BUILD_DIR/CMakeFiles/" "$BUILD_DIR/CMakeCache.txt"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

if using_conan; then
    if [[ -n "$CONAN_PROFILE" ]]; then
        conan install "$SRC_DIR" -pr="$CONAN_PROFILE" --build=missing
    else
        conan install "$SRC_DIR" --build=missing
    fi
    if [[ $? -ne 0 ]]; then
        echo "ERROR: Conan failed"
        exit $(false)
    fi
    cmake "$SRC_DIR" -D CMAKE_TOOLCHAIN_FILE=conan_paths.cmake
else
    cmake "$SRC_DIR"
fi

if [[ $? -ne 0 ]]; then
    echo "cmake failed to configure"
    exit 1
fi

cmake --build .
