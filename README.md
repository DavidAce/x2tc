# x2tc

`x2tc` is a compiled C++23 library for compensated floating point arithmetic in Eigen matrices and tensors.
It stores each number as a pair `(hi, lo)` of ordinary floating point values, and uses this
representation to achieve higher precision in matrix-matrix multiplications and tensor contractions.


## Requirements

  * CMake 3.24 or newer.
  * A C++23 compiler.
  * Eigen 3.4 or later, before Eigen 6.
  * OpenMP.

The default build enables `double` and `std::complex<double>`. Other scalar
widths can be enabled with CMake options.

## Example

```cpp
#include <x2tc/x2tc.h>

#include <Eigen/Core>
#include <unsupported/Eigen/CXX11/Tensor>

int main() {
    Eigen::Tensor<double, 3> A(2, 2, 3);
    Eigen::Tensor<double, 3> B(3, 2, 2);
    x2::Tensor<double, 4>    C(2, 2, 2, 2);

    A.setRandom();
    B.setRandom();
    C.setZero();

    auto A_mat = x2::as_const_matrix(A, {0, 1}, {2});
    auto B_mat = x2::as_const_matrix(B, {0}, {1, 2});
    auto C_mat = x2::as_matrix_x2<double, 4>(C, {0, 1}, {2, 3});

    x2::gemm_x2(C_mat, A_mat, B_mat);

    Eigen::Tensor<double, 4> result = C.to_EigenTensor();
}
```

More complete examples are available in `examples/`. They can be built with:

```bash
cmake -S . -B build/examples \
    -DCMAKE_BUILD_TYPE=Release \
    -DX2TC_BUILD_EXAMPLES=ON
cmake --build build/examples --target build-all-examples
```

## Install

Clone the repository and configure with CMake:

```bash
git clone https://github.com/DavidAce/x2tc.git
cd x2tc
cmake -S . -B build/release \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/path/to/x2tc
cmake --build build/release
ctest --test-dir build/release
cmake --install build/release
```

This builds a shared release library and installs `x2tc` under
`/path/to/x2tc`. This command uses the default `find` dependency mode, so Eigen
and OpenMP must already be visible to CMake.

## Presets

The package manager modes require several CMake variables to be set together.
For that reason, the presets are the preferred way to use the CMake and Conan
dependency providers.

| Preset          | Dependency mode | Description                                              |
|:----------------|:----------------|:---------------------------------------------------------|
| `release-find`  | `find`          | Use dependencies already visible to `find_package`.      |
| `release-cmake` | `cmake`         | Build and install Eigen with CMake during configuration. |
| `release-conan` | `conan`         | Install Eigen with Conan during configuration.           |

For example:

```bash
cmake --preset release-conan
cmake --build --preset release-conan
ctest --preset release-conan
cmake --install build/release-conan
```

## Use From CMake

After installation:

```cmake
find_package(x2tc REQUIRED)

add_executable(my_program main.cpp)
target_link_libraries(my_program PRIVATE x2tc::x2tc)
```

## CMake Options

| Option                 | Default | Description                                                                                                                    |
|:-----------------------|:--------|:-------------------------------------------------------------------------------------------------------------------------------|
| `X2TC_ENABLE_32BIT`    | `OFF`   | Compile `float` and `std::complex<float>` instantiations.                                                                      |
| `X2TC_ENABLE_64BIT`    | `ON`    | Compile `double` and `std::complex<double>` instantiations.                                                                    |
| `X2TC_ENABLE_80BIT`    | `OFF`   | Compile `long double` and `std::complex<long double>` instantiations.                                                          |
| `X2TC_ENABLE_128BIT`   | `OFF`   | Compile `std::float128_t` and `std::complex<std::float128_t>` instantiations. Requires compiler support for `std::float128_t`. |
| `X2TC_ENABLE_TESTS`    | `OFF`   | Build the test programs and enable CTest.                                                                                      |
| `X2TC_BUILD_EXAMPLES`  | `OFF`   | Build the example programs in `examples/`.                                                                                     |
