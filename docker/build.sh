#!/bin/bash

# Rebuild Docker image with Emscripten installed

# First, get path to the Dockerfile
SOURCE_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd)

# Second, use the path in "build" command
docker build --rm -t sfalexrog/emcc:latest ${SOURCE_DIR}
