from conans import ConanFile, CMake, tools


class StdextshellConan(ConanFile):
    name = "stdext-shell"
    version = "1.0"
    license = "http://github.com/srand/stdext-shell/LICENSE"
    url = "http://github.com/srand/stdext-shell"
    description = "Simple shell command parser for interactivity in C++ applications."
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    exports_sources = "*"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["stdext-shell"]
