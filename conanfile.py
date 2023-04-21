from conan import ConanFile
from conan.tools.cmake import CMake


class Recipe(ConanFile):
    v = open("version.txt").readline().strip()
    name = "poggles"
    version = v

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps", "VirtualRunEnv"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports = "version.txt"
    exports_sources = "version.txt", "CMakeLists.txt", "CMakePresets.json", "cmake/*", "include/*", "source/*"

    def configure(self):
        self.options["glad"].gl_profile = "core"
        self.options["glad"].gl_version = "4.6"

    def layout(self):
        self.folders.generators = "conan"

    def requirements(self):
        requirements = [
            "glad/0.1.36",
            "stb/cci.20220909",
            "opengl/system",
            "doctest/2.4.10"
        ]
        for r in requirements:
            self.requires(r)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["poggles"]
