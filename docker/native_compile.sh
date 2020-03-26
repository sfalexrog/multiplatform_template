#!/bin/bash

# Compile the project using native tools

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd)
SOURCE_DIR="${SCRIPT_DIR}/../"

docker run -it --rm -w "${SOURCE_DIR}" \
    --volume ${SOURCE_DIR}:${SOURCE_DIR}:rw \
    sfalexrog/sdl2builder:latest \
    /bin/bash -c 'mkdir -p nbuild; \
        cd nbuild; \
        cmake ..; \
        make -j$(nproc);'
