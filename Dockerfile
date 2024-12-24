FROM ubuntu:22.04

RUN apt update -y && apt upgrade -y

RUN apt install -y curl wget git

# build-esenials - make, gvv g++, libc, libstdc++
RUN apt install -y build-essential

# python
RUN apt install -y python3 python3-pip

RUN apt install -y bc cmake pkg-config meson bear

# flatbuffer
RUN apt install -y \
    libflatbuffers-dev \
    flatbuffers-compiler