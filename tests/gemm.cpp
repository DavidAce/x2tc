#include <x2tc/x2tc.h>

#include <cmath>
#include <iostream>

namespace {
    void require(bool ok, const char *msg) {
        if(!ok) {
            std::cerr << msg << '\n';
            std::exit(1);
        }
    }
}

int main() {
    Eigen::Tensor<fp64, 3> A(2, 2, 3);
    Eigen::Tensor<fp64, 3> B(3, 2, 2);
    x2::Tensor<fp64, 4>    C(2, 2, 2, 2);

    for(Eigen::Index i = 0; i < A.size(); ++i) A.data()[i] = static_cast<fp64>(i + 1) / 10.0;
    for(Eigen::Index i = 0; i < B.size(); ++i) B.data()[i] = static_cast<fp64>(i + 2) / 20.0;
    C.setZero();

    auto A_mat = x2::as_const_matrix(A, {0, 1}, {2});
    auto B_mat = x2::as_const_matrix(B, {0}, {1, 2});
    auto C_mat = x2::as_matrix_x2<fp64, 4>(C, {0, 1}, {2, 3});

    x2::gemm_x2(C_mat, A_mat, B_mat);

    for(Eigen::Index r = 0; r < C_mat.rows(); ++r) {
        for(Eigen::Index c = 0; c < C_mat.cols(); ++c) {
            fp64 expected = 0.0;
            for(Eigen::Index k = 0; k < A_mat.cols(); ++k) expected += A_mat(r, k) * B_mat(k, c);
            const fp64 actual = C_mat.hi(r, c) + C_mat.lo(r, c);
            require(std::abs(actual - expected) < 1e-12, "gemm_x2 result mismatch");
        }
    }

    return 0;
}

