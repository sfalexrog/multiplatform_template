#!/bin/bash

# Compile the project using emscripten

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd)
SOURCE_DIR="${SCRIPT_DIR}/../"

docker run -it --rm -w "${SOURCE_DIR}" \
    --volume ${SOURCE_DIR}:${SOURCE_DIR}:rw \
    --volume ${SOURCE_DIR}/.emscripten_cache:/root/.emscripten_cache:rw \
    sfalexrog/emcc:latest \
    /bin/bash -c '. /root/emsdk-master/emsdk_env.sh; \
        mkdir -p embuild; \
        cd embuild; \
        emcmake cmake ..; \
        emmake make -j$(nproc);'
