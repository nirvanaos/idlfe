from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout

class idlfeRecipe(ConanFile):
    name = "nirvanaos.idlfe"
    version = "3.3.0"

    # Optional metadata
    license = "LGPL-3.0-or-later"
    author = "Igor Popov popov.nirvana@gmail.com"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "IDL compiler front-end library"
    topics = ("CORBA", "IDL", "compiler", "front-end")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "example/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["idlfe", "decNumber"]
    