from conans import ConanFile, CMake

class Sqlite3ccConan(ConanFile):
    name = "SQLite3cc"
    version = "0.1.1"
    url = "https://github.com/monsdar/sqlite3cc-cmake"
    license = "GNU LESSER GENERAL PUBLIC LICENSE Version 3"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports = "*"

    def config(self):
        self.requires.add("SQLite3/3.12.1@monsdar/testing")
        self.requires.add("Boost/1.60.0@lasote/stable")
    
    def source(self):
        #this packet brings all necessary sources with it
        pass

    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake %s %s' % (self.conanfile_directory, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include", src="%s/sqlite3cc-0.1.1/include" % self.conanfile_directory)
        self.copy("*.lib", dst="lib", src="lib")
        self.copy("*.a", dst="lib", src="lib")

    def package_info(self):
        self.cpp_info.libs = ["sqlite3cc-cmake"]
        