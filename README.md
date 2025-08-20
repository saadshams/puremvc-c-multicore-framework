## [PureMVC](https://puremvc.org) C Multicore Framework

PureMVC is a lightweight framework for creating applications based upon the classic [Model-View-Controller](http://en.wikipedia.org/wiki/Model-view-controller) design meta-pattern. It supports [modular programming](http://en.wikipedia.org/wiki/Modular_programming) through the use of [Multiton](http://en.wikipedia.org/wiki/Multiton) Core actors instead of the [Singletons](http://en.wikipedia.org/wiki/Singleton_pattern).
* [API Docs]()
* [Unit Tests]()

## Platforms / Technologies
* [C](https://en.wikipedia.org/wiki/C_(programming_language))
* [Arduino](https://en.wikipedia.org/wiki/Arduino)
* [Internet of things](https://en.wikipedia.org/wiki/Internet_of_things)
* [WebAssembly](https://en.wikipedia.org/wiki/WebAssembly)

## License
* PureMVC Multicore Framework for C - Copyright © 2025 [Saad Shams](https://www.linkedin.com/in/muizz/)
* PureMVC - Copyright © 2025 [Futurescale, Inc](http://futurescale.com)
* All rights reserved.

* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of Futurescale, Inc., PureMVC.org, nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


cmake -S . -B build
cmake --build build
cd build
ctest


mkdir build && cd build
cmake ..
make
ctest

To build with tests:
cmake -B build -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build

brew install cmake
cmake --version

**Ownership Model Overview**
This library follows a clear ownership model for managing memory and data across core components like Proxy and Mediator. 
Objects are either **providers** that allocate and supply data, **owners** that take responsibility for freeing that data, 
or **borrowers** that access data without owning it. 
This approach helps prevent memory leaks and ensures safe, predictable resource management throughout the framework.

**Proxy**
Proxies receive heap-allocated data from providers and become the **owners** of that data.
Once a Proxy takes ownership, the provider must not free the data—it's the Proxy’s responsibility 
to manage and free it.
This clear ownership prevents memory errors and defines unambiguous lifecycles.
removeProxy (called becomes the owner and is responsible to free)

**Mediator**
Mediators hold **weak (non-owning) references** to UI components, which the operating system may create or destroy dynamically based on user interaction.
Because Mediators do not own these components, they do **not** free or manage their lifetimes.
This avoids dangling pointers and keeps the interaction with UI safe and stable.

**Command**
Commands are **allocated dynamically** when triggered by notifications and freed immediately after execution.
They are intentionally **short-lived**, existing only for the duration of their task to minimize resource consumption.

**Note:**
The Proxy's data must be heap-allocated because the library assumes ownership 
and will free() them when no longer needed. 
Passing non-heap memory (e.g., stack or static variables) will cause crashes due to invalid free() operations.

Notification: body is borrowed.

return const

readonly return: const char *name
readonly param: const char *name

```shell
cmake -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DENABLE_SANITIZERS=ON \
  -DCMAKE_C_FLAGS="-Wall -Werror -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address,undefined" \
  -DCMAKE_SHARED_LINKER_FLAGS="-fsanitize=address,undefined"
```

Windows
```shell
cmake -B build \
  -G "Visual Studio 17 2022" \
  -A x64 \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_FLAGS="/W4 /WX /std:c11" \
  -DCMAKE_EXE_LINKER_FLAGS="" \
  -DCMAKE_SHARED_LINKER_FLAGS=""
```

## Installation

1. vcpkg.json
`{
    "name": "application",
    "version": "1.0.0",
    "dependencies": [
        "puremvc"
    ]
}`

2. Configure CMake to use the vcpkg toolchain by passing this:
`-DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake`

3. vcpkg install


Reference:
[Versioning reference](https://learn.microsoft.com/en-us/vcpkg/users/versioning)
