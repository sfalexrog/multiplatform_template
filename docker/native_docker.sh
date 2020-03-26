#!/bin/bash

# Rebuild Docker image with SDL2 dependencies installed

# First, get path to the Dockerfile
SOURCE_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd)

# Second, use the path in "build" command
docker build --rm -t sfalexrog/sdl2builder:latest -f Dockerfile_native ${SOURCE_DIR}
