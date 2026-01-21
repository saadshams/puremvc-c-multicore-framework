FROM gcc:latest

# Install required tools
RUN apt update && apt install -y cmake git zip

# Install VCPKG
WORKDIR /opt
RUN git clone https://github.com/microsoft/vcpkg.git
WORKDIR /opt/vcpkg
RUN ./bootstrap-vcpkg.sh

# Add VCPKG to PATH
ENV PATH="/opt/vcpkg:${PATH}"

WORKDIR /app
