#!/usr/bin/env bash

set -ex

mkdir build
cd build

# See more options: https://github.com/jslee02/SimBenchmark/blob/dart/CMakeLists.txt#L18
cmake .. \
  -DBENCHMARK_BULLET_MULTIBODY=OFF \
  -DBENCHMARK_MUJOCO=OFF \
  -DBENCHMARK_ODE=OFF \
  -DBENCHMARK_RAISIM=OFF \
  -DBENCHMARK_DART=ON

make -j4
