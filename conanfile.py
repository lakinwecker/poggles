from conan import ConanFile
from conan.tools.cmake import CMake

import json

deps = {}

class Recipe(ConanFile):
    v = open("version.txt").readline().strip()
    d = open("dependencies.json")
    global deps
    deps = json.load(d)

    name = "poggles"
    version = v

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps", "VirtualRunEnv"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports = "version.txt", "dependencies.json"
    exports_sources = "version.txt", "CMakeLists.txt", "CMakePresets.json", "cmake/*", "include/*", "source/*"

    def configure(self):
        self.options["glad"].gl_profile = "core"
        self.options["glad"].gl_version = "4.6"

    def layout(self):
        self.folders.generators = "conan"

    def requirements(self):
        global deps
        for n, v in deps.items():
            self.requires(n + "/" + v["current"])

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["poggles"]
