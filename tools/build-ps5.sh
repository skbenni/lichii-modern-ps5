#!/usr/bin/env bash
set -euo pipefail

: "${PS5_PAYLOAD_SDK:?Set PS5_PAYLOAD_SDK to your ps5-payload-sdk checkout}"
: "${PS5_TOOLCHAIN:?Set PS5_TOOLCHAIN to the SDK CMake toolchain file}"

cmake -S . -B build-ps5 \
  -DLICHII_PS5=ON \
  -DCMAKE_TOOLCHAIN_FILE="$PS5_TOOLCHAIN" \
  -DPS5_PAYLOAD_SDK="$PS5_PAYLOAD_SDK"
cmake --build build-ps5 --parallel

mkdir -p dist/PPSA99011
cp build-ps5/eboot dist/PPSA99011/eboot.bin
cp ui/*.rcss ui/*.rml dist/PPSA99011/
