# PureMVC C Multicore — Developer Guide

This document is intended for **developers using PureMVC implemented in C**. It focuses on the internal architecture, memory management rules, and development practices specific to the C version of the framework.

---

## Overview

PureMVC for C is designed to provide a predictable, lightweight, and safe implementation of the PureMVC pattern in a manual-memory-management environment. As such, **clear ownership rules** and **well-defined lifecycles** are fundamental to the framework’s design.

This guide explains how core components interact, how memory ownership is transferred, and what contributors must follow to avoid leaks, double frees, or dangling pointers.

---

### C Development Environment Setup

#### Windows (MSVC Toolchain)

Install Visual Studio Build Tools (MSVC): https://visualstudio.microsoft.com/downloads

**Select:** C++ build tools, MSVC v143, Windows 10/11 SDK, CMake, Ninja

#### Add MSVC tools to PATH

`C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\<VERSION>\bin\Hostx64\x64`

#### Verify Installation

```shell
cl
link
cmake --version
ninja --version
```

#### macOS (Clang Toolchain)

**Install Xcode Command Line Tools**

`xcode-select --install`

#### Verify compiler tools
```shell
clang --version
make --version
```

**Install CMake**

```shell
brew install cmake
cmake --version
```

### 🚀 Install VCPKG

* **Clone the repository:**  
  ```shell
  git clone https://github.com/microsoft/vcpkg.git
  cd vcpkg
  ```

* **Bootstrap vcpkg:**  
  Linux / macOS: `./bootstrap-vcpkg.sh`  
  Windows (PowerShell or CMD): `.\bootstrap-vcpkg.bat`  
  Optional (Visual Studio integration on Windows): `.\vcpkg integrate install`  


* Set environment variables (Linux / macOS)

  ```shell
  echo 'export VCPKG_ROOT="/path/to/vcpkg"' >> ~/.zshrc # Set VCPKG_ROOT
  echo 'export PATH="$VCPKG_ROOT:$PATH"' >> ~/.zshrc # Add VCPKG to your PATH
  source ~/.zshrc && vcpkg --version # reload and check the version
  ```

## 🛠️ Build
### Debug
```shell
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
ctest -C Debug
```

### Debug GDB

```shell
mkdir -p build && cd build

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake ..
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_TOOLCHAIN_FILE=/Users/sshams/Documents/microsoft/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --parallel
ctest --test-dir build --output-on-failure --verbose

rm -rf build

cmake -S . -B build-gcc \
-DCMAKE_TOOLCHAIN_FILE=/Users/sshams/Documents/microsoft/vcpkg/scripts/buildsystems/vcpkg.cmake \
-DCMAKE_C_COMPILER=gcc \
-DCMAKE_CXX_COMPILER=g++-14 \
-DCMAKE_BUILD_TYPE=Release \
-DBUILD_TESTS=ON \
-DEXAMPLE_TESTS=ON \
&& cmake --build build-gcc \
&& ctest --test-dir build-gcc --output-on-failure --verbose

cmake -S . -B build-gcc \
-DCMAKE_TOOLCHAIN_FILE=/Users/sshams/Documents/microsoft/vcpkg/scripts/buildsystems/vcpkg.cmake \
-DCMAKE_C_COMPILER=gcc \
-DCMAKE_CXX_COMPILER=g++ \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_C_FLAGS="-g" \
-DBUILD_TESTS=ON \
-DEXAMPLE_TESTS=ON \
&& cmake --build build-gcc \
&& ctest --test-dir build-gcc --output-on-failure --verbose

cd build-gcc/test
gdb ./ControllerTest
```

### Debug + Sanitizer
```shell
cmake -B build \
 -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake \
 -DCMAKE_C_FLAGS="-Wall -Werror -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer" \
 -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address,undefined" 

cmake --build build
ctest --test-dir build --output-on-failure
```

### Release
```shell
mkdir -p build-release && cd build-release
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
ctest -C Release
```

### Debug (Windows)
```shell
cmake -B build \
  -G "Visual Studio 17 2022" \
  -A x64 \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_TESTS=ON \
  -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_C_FLAGS="/W4 /WX /std:c11" \
  -DCMAKE_EXE_LINKER_FLAGS="" \
  -DCMAKE_SHARED_LINKER_FLAGS=""
```



### Documentation
```shell
brew install doxygen # Installation
doxygen -g # Create Doxyfile
doxygen && open docs/index.html # To generate docs
```

---

## Ownership Model

PureMVC follows a strict and explicit ownership model for managing memory and data across its core components (Proxy, Mediator, Command).

Objects interacting with the framework fall into one of three roles:

* **Providers** – Allocate and supply data
* **Owners** – Take responsibility for freeing data
* **Borrowers** – Access data without owning or freeing it

This separation ensures:

* No ambiguous lifetimes
* No implicit ownership transfer
* Predictable and safe resource cleanup

---

## Proxy

### Role

Proxies are responsible for **owning and managing application data**.

### Ownership Rules

* Proxies receive **heap-allocated data** from providers
* Once received, the Proxy becomes the **sole owner** of that data
* Providers **must not** free the data after passing it to a Proxy
* The Proxy is responsible for freeing the data during it's cleanup

### Removal Semantics

* When `removeProxy` is called:

    * Ownership of the Proxy’s data is transferred to the **caller**
    * The caller becomes responsible for freeing the data

This explicit transfer avoids hidden frees and allows callers to safely reuse or dispose of the data.

### Important Constraint

* **Proxy data must always be heap-allocated**
* Passing stack or static memory will result in invalid `free()` calls and crashes

---

## Mediator

### Role

Mediators coordinate interaction between the PureMVC system and UI components.

### Ownership Rules

* Mediators hold **non-owning references** to UI components
* UI components are often created and destroyed dynamically by the operating system or UI framework
* Mediators **must not**:

    * Allocate UI components
    * Free UI components
    * Assume lifetime control over UI elements

### Rationale

By treating UI components as borrowed references, Mediators:

* Avoid dangling pointers
* Remain resilient to UI lifecycle changes
* Maintain a clean separation between UI ownership and application logic

---

## Command

### Role

Commands encapsulate executable logic triggered by notifications.

### Lifecycle

* Commands are **allocated dynamically** when a notification triggers them
* They execute their logic immediately
* They are **freed immediately after execution**

### Design Intent

* Commands are intentionally **short-lived**
* They should not retain long-term state
* This minimizes memory usage and avoids lifecycle complexity

---

## Notification

### Body Ownership

* The `Notification`’s `body` is **borrowed**, not owned; however, if setBody is called with new data, the previously set body is freed
* A consumer of the Notification’s body may explicitly assume ownership and is then responsible for freeing it. If there are multiple consumers, exactly one must be designated as the owner, while all others treat the body as borrowed.* Commands and Mediators must **not free** the body
* Commands and Mediators must not free the body unless ownership has been explicitly transferred to them
* The `sendNotification` mechanism automatically frees the `Notification` which in turn frees the `name` and `type` fields at the end of dispatch, **but not the** `body`.

This ensures notifications remain lightweight carriers of information rather than owners of resources.

---

## Memory Management Rules (Summary)

* Only free memory you **explicitly own**
* Proxies own their data until removed
* Mediators never own UI components
* Commands own themselves only during execution
* Notification bodies are borrowed references

Violating these rules will result in:

* Double frees
* Use-after-free bugs
* Undefined behavior

---

## Development Guidelines

* Prefer clarity over convenience when transferring ownership
* Document ownership expectations in function comments
* Avoid hidden allocations or implicit frees
* Keep Commands small and stateless
* Validate heap ownership when debugging crashes

---

## Final Note

PureMVC’s C implementation relies on discipline rather than a garbage collector. Following the ownership model strictly is not optional—it is essential for correctness and stability.
