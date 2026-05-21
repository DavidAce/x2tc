from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.scm import Version
from conan.errors import ConanInvalidConfiguration

required_conan_version = ">=2.0"


class X2tcConan(ConanFile):
    name = "x2tc"
    version = "0.1.0"
    description = "Standalone compensated twofold arithmetic and tensor contraction library"
    homepage = "https://github.com/DavidAce/x2tc"
    url = "https://github.com/DavidAce/x2tc"
    author = "DavidAce <aceituno@kth.se>"
    topics = ("linear-algebra", "tensor", "compensated-arithmetic")
    license = "MIT"
    settings = "os", "compiler", "build_type", "arch"
    generators = ("CMakeDeps", "CMakeConfigDeps")
    no_copy_source = True
    short_paths = True

    @property
    def _compilers_minimum_version(self):
        return {
            "gcc": "12",
            "Visual Studio": "15.7",
            "clang": "16",
            "apple-clang": "10",
        }

    def validate(self):
        if self.settings.compiler.get_safe("cppstd"):
            check_min_cppstd(self, 23)
        minimum_version = self._compilers_minimum_version.get(str(self.settings.compiler), False)
        if minimum_version and Version(self.settings.compiler.version) < minimum_version:
            raise ConanInvalidConfiguration("x2tc requires C++23, which your compiler does not support.")
        if not minimum_version:
            self.output.warning("x2tc requires C++23. Your compiler is unknown. Assuming it supports C++23.")

    def requirements(self):
        self.requires("eigen/[>=3.4.0 <6.0.0]", force=True, transitive_headers=True)
