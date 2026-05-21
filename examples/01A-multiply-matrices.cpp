#include <x2tc/x2tc.h>

#include <Eigen/Core>
#include <cmath>
#include <print>
#include <unsupported/Eigen/CXX11/Tensor>

int main() {
    // This example computes C = A * B with x2 compensated arithmetic.
    // The input matrices are ordinary Eigen tensors of rank 2.
    Eigen::Tensor<double, 2> A(2, 3);
    Eigen::Tensor<double, 2> B(3, 2);

    // The output is stored as an x2 tensor. Each entry has a high part and a
    // low part. Their sum is the compensated result.
    x2::Tensor<double, 2> C(2, 2);

    for(Eigen::Index i = 0; i < A.size(); ++i) A.data()[i] = static_cast<double>(i + 1) / 10.0;
    for(Eigen::Index i = 0; i < B.size(); ++i) B.data()[i] = static_cast<double>(i + 2) / 20.0;
    C.setZero();

    // Interpret the rank-2 tensors as matrices. Axis 0 is the row index, and
    // axis 1 is the column index.
    x2::ConstTensorAsMatrixView<double, 2> A_mat = x2::as_const_matrix(A, {0}, {1});
    x2::ConstTensorAsMatrixView<double, 2> B_mat = x2::as_const_matrix(B, {0}, {1});
    x2::X2TensorAsMatrixView<double, 2>    C_mat = x2::as_matrix_x2<double, 2>(C, {0}, {1});

    // Compute C_mat = A_mat * B_mat. The high and low parts are written into C.
    x2::gemm_x2(C_mat, A_mat, B_mat);

    Eigen::Tensor<double, 2> result = C.to_EigenTensor();

    std::println("C = A * B");
    for(Eigen::Index row = 0; row < result.dimension(0); ++row) {
        for(Eigen::Index col = 0; col < result.dimension(1); ++col) {
            std::print("{} ", result(row, col));
        }
        std::println("");
    }

    // Check one entry against the ordinary dot product.
    double expected = 0.0;
    for(Eigen::Index k = 0; k < A.dimension(1); ++k) expected += A(0, k) * B(k, 0);
    if(std::abs(result(0, 0) - expected) > 1e-12) return 1;

    return 0;
}
