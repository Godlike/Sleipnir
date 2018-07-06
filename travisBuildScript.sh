#!/bin/sh
set -e

mkdir $BUILD_DIR
cd $BUILD_DIR

CMAKE_FLAGS="-DCMAKE_CXX_COMPILER=$COMPILER -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE -DSLEIPNIR_SHLIBDEPS=OFF"

if [ -n "$COMPILER_FLAGS" ]; then
    CMAKE_FLAGS="$CMAKE_FLAGS -DCMAKE_CXX_FLAGS=$COMPILER_FLAGS"
fi

cmake $CMAKE_FLAGS ..

# Unicorn's dependency Assimp eats too much RAM when built in parallel
make -j2 package
