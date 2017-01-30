from conans import ConanFile, CMake, tools, ConfigureEnvironment
from enviroment import CMakePackage
from enviroment import CMakeHelper
import os


class MTGPackage(CMakePackage):
    name = "mtg"
    version = "HEAD"
    license = "MIT"
    url = "none"
    settings = {"os": ["Windows"],
    "arch":["x86"],
    "compiler":{
    "gcc" : {"version" : ["5.3"]},
    "Visual Studio" : {"version" : ["14"]}
    },
    "build_type" : ["Debug","Release"]
    }
    build_policy = "missing"
    exports = "*"
    requires = "qt/5.7.0@rit/stable"