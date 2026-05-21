#include <x2tc/x2tc.h>

#include <cmath>
#include <print>
#include <type_traits>

namespace {
    void require(bool ok, const char *msg) {
        if(!ok) {
            std::println(stderr, "{}", msg);
            std::exit(1);
        }
    }
}

int main() {
    static_assert(std::is_same_v<fp64, double>);
    static_assert(std::is_same_v<cx64, std::complex<double>>);
    static_assert(Eigen::NumTraits<fp64>::IsComplex == 0);
    static_assert(Eigen::NumTraits<cx64>::IsComplex == 1);

    x2::Matrix<fp64> matrix(2, 2);
    matrix.hi().setConstant(1.0);
    matrix.lo().setConstant(0.25);
    matrix.renorm();
    require(matrix.rows() == 2 && matrix.cols() == 2, "matrix shape mismatch");
    require(matrix.allFinite(), "matrix has non-finite values");
    require(std::abs(matrix.to_MatrixType()(0, 0) - 1.25) < 1e-14, "matrix cached sum mismatch");

    x2::Tensor<fp64, 3> tensor(2, 3, 4);
    tensor.hi().setConstant(2.0);
    tensor.lo().setConstant(0.5);
    tensor.renorm();
    require(tensor.size() == 24, "tensor size mismatch");
    require(tensor.allFinite(), "tensor has non-finite values");
    require(std::abs(tensor.to_EigenTensor()(0, 0, 0) - 2.5) < 1e-14, "tensor cached sum mismatch");

    return 0;
}
