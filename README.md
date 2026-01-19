## [PureMVC](https://puremvc.org) C Multicore Framework

[![License](https://img.shields.io/badge/license-BSD--3--Clause-blue.svg)](LICENSE)
[![CMake on multiple platforms](https://github.com/saadshams/puremvc-c-multicore-framework/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/open-source-patterns/collection/actions/workflows/cmake-multi-platform.yml)

PureMVC is a lightweight framework for creating applications based upon the classic [Model-View-Controller](http://en.wikipedia.org/wiki/Model-view-controller) design meta-pattern. It supports [modular programming](http://en.wikipedia.org/wiki/Modular_programming) through the use of [Multiton](http://en.wikipedia.org/wiki/Multiton) Core actors instead of the [Singletons](http://en.wikipedia.org/wiki/Singleton_pattern).
* [API Docs](https://PureMVC.github.io/puremvc-c-multicore-framework)

## 📦 Installation
1. Add `"puremvc"` to the `dependencies` in your `vcpkg.json` file
  ```json 
  {
      "name": "application",
      "version": "1.0.0",
      "dependencies": [
          "puremvc"
      ]
  }
  ```
              
2. Configure CMake to use the vcpkg toolchain by passing this:
   `-DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake`

3. Run: `vcpkg install` to install the dependency

## Platforms / Technologies
* [C](https://en.wikipedia.org/wiki/C_(programming_language))
* [Arduino](https://en.wikipedia.org/wiki/Arduino)
* [Internet of things](https://en.wikipedia.org/wiki/Internet_of_things)
* [WebAssembly](https://en.wikipedia.org/wiki/WebAssembly)

## License
* PureMVC Multicore Framework for C - Copyright © 2026 [Saad Shams](https://www.linkedin.com/in/muizz/)
* PureMVC - Copyright © 2026 [Futurescale, Inc](http://futurescale.com)
* All rights reserved.

* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of Futurescale, Inc., PureMVC.org, nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
