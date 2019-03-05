from conans import ConanFile, CMake

class Sqlite3ccConan(ConanFile):
    name = "SQLite3cc"
    version = "0.1.1"
    url = "https://github.com/monsdar/sqlite3cc-cmake"
    license = "GNU LESSER GENERAL PUBLIC LICENSE Version 3"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports = "*"

    def requirements(self):
        self.requires.add("sqlite3/3.27.2@bincrafters/stable")
        self.requires.add("boost/1.69.0@conan/stable")
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.install()

    def package(self):
        self.copy("*.h", dst="include", src="%s/sqlite3cc-0.1.1/include" % self.source_folder)
        self.copy("*.lib", dst="lib", src="lib")
        self.copy("*.a", dst="lib", src="lib")

    def package_info(self):
        self.cpp_info.libs = ["sqlite3cc"]
        