# Use an official Ubuntu as a parent image
FROM ubuntu:20.04

# Set environment variables to avoid interactive installation issues
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    libgsl-dev \
    zlib1g-dev \
    git \
    wget \
    unzip \
    libopenblas-dev \
    && rm -rf /var/lib/apt/lists/*

# Clone the GEMMA repository and build it
WORKDIR /opt
RUN git clone https://github.com/genetics-statistics/GEMMA.git gemma \
    && cd gemma \
    && make -j4

# Set the path
ENV PATH="/opt/gemma/bin:${PATH}"

# Set the working directory
WORKDIR /data

# Set the ENTRYPOINT to GEMMA
ENTRYPOINT ["gemma"]
