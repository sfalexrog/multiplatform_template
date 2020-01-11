FROM debian:buster-slim

RUN apt-get update && apt-get install -y \ 
    build-essential \
    ca-certificates \
    cmake \
    curl \
    git \
    python \
    python-pip \
    python3 \
    python3-pip \
    tar \
  && apt-get clean

RUN python2 -m pip install --no-cache-dir --upgrade pip \
  && python2 -m pip install --no-cache-dir flake8==3.7.8

RUN python3 -m pip install --no-cache-dir --upgrade pip \
  && python3 -m pip install --no-cache-dir flake8==3.7.8

RUN curl -# -L -o ~/emsdk-master.tar.gz https://github.com/emscripten-core/emsdk/archive/master.tar.gz \
  && tar -C ~ -xf ~/emsdk-master.tar.gz \
  && cd ~/emsdk-master \
  && ./emsdk update-tags \
  && ./emsdk install latest \
  && ./emsdk activate latest \
  && rm -rf ~/emsdk-master.tar.gz
