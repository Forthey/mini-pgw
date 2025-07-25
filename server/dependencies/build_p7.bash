#!/bin/bash

P7_URL="https://baical.net/files/libP7Client_v5.6.zip"
BUILD_DIR="$1/_deps/"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

wget "$P7_URL" -O libP7Client.zip

unzip libP7Client.zip -d p7_source -A

cd p7_source

mkdir _build_

cd _build_

cmake .. -DCMAKE_INSTALL_PREFIX="${BUILD_DIR}/p7_source"

make

make install
