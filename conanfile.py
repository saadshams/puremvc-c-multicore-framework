from conans import ConanFile

class Foo(ConanFile):
    name = "foo"
    version = "0.1"
    license = "MIT"
    url = "https://github.com/username/foo.git"
    description = "My Open Source Library"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"

    def source(self):
        self.run("git clone https://github.com/conan-io/hello.git")
        tools.replace_in_file("foo/CMakeLists.txt", "PROJECT(MyLibrary)",
                              '''PROJECT(MyLibrary)
        include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
        conan_basic_setup()''')

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_dir="src")
        cmake.build()
        cmake.install()

    def package(self):
        self.copy("*.h", dst="include", src="hello")
        self.copy("*hello.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["mylibrary"]