ARG DOCKER_IMAGE=debian:unstable
FROM $DOCKER_IMAGE

RUN apt-get update && apt-get install -y \
    python3-dev python3-pip python3-setuptools \
    pybind11-dev \
    libssl-dev \
    libboost-all-dev  \
    libopencv-dev \
    libfmt-dev \
    libbenchmark-dev \
    libgtest-dev \
    llvm-dev \
    nlohmann-json3-dev \
    libpoco-dev \
    libx11-dev libxrandr-dev xinput libxcb-xinput-dev \
    libxcursor-dev libxi-dev libxinerama-dev libxkbcommon-dev \
    libvulkan-dev freeglut3-dev \
    ninja-build cmake build-essential \
    valgrind git gdb ffmpeg \
    ccache distcc \
    && apt-get -y autoremove --purge \
    && rm -rf /var/lib/apt/lists/*

# Add: parallel cloc

WORKDIR /app

#ENV TERM xterm-256color
#SHELL ["/bin/bash", "-c", "-l"]
#ENTRYPOINT ["bash", "-l"]

#CMD [ "make" ]
