#!/usr/bin/env bash
set -euo pipefail
zoo=${1:-$HOME/rockchip/rknn_model_zoo}
compiler=${2:-$HOME/rockchip/arm-gnu-toolchain-12.2.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-g++}
project=$(cd "$(dirname "$0")/.." && pwd)
build="$zoo/build/build_rknn_yolo11_demo_rk3576_linux_aarch64_Release"
mkdir -p "$project/.yolo11_run"
"$compiler" -O3 -DNDEBUG "$project/tools/yolo11_benchmark.cc" \
  -I"$zoo/examples/yolo11/cpp" -I"$zoo/utils" -I"$zoo/3rdparty/rknpu2/include" \
  "$build/CMakeFiles/rknn_yolo11_demo.dir/postprocess.cc.o" \
  "$build/CMakeFiles/rknn_yolo11_demo.dir/rknpu2/yolo11.cc.o" \
  "$build/utils.out/libimageutils.a" "$build/utils.out/libfileutils.a" \
  "$zoo/3rdparty/rknpu2/Linux/aarch64/librknnrt.so" -ldl \
  "$zoo/3rdparty/librga/Linux/aarch64/librga.a" \
  "$zoo/3rdparty/jpeg_turbo/Linux/aarch64/libturbojpeg.a" \
  -o "$project/.yolo11_run/yolo11_benchmark"
