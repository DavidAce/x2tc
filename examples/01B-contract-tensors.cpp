#include <x2tc/x2tc.h>

#include <Eigen/Core>
#include <cmath>
#include <print>
#include <unsupported/Eigen/CXX11/Tensor>

int main() {
    // This example contracts one index:
    //
    //     C(i,j,l,m) = sum_k A(i,j,k) B(k,l,m)
    //
    // The contraction is expressed as a matrix multiplication by grouping
    // tensor axes into row and column axes.
    Eigen::Tensor<double, 3> A(2, 2, 3);
    Eigen::Tensor<double, 3> B(3, 2, 2);
    x2::Tensor<double, 4>    C(2, 2, 2, 2);

    for(Eigen::Index i = 0; i < A.size(); ++i) A.data()[i] = static_cast<double>(i + 1) / 10.0;
    for(Eigen::Index i = 0; i < B.size(); ++i) B.data()[i] = static_cast<double>(i + 2) / 20.0;
    C.setZero();

    // A(i,j,k) is viewed as a matrix with rows (i,j) and columns k.
    x2::ConstTensorAsMatrixView<double, 3> A_mat = x2::as_const_matrix(A, {0, 1}, {2});

    // B(k,l,m) is viewed as a matrix with rows k and columns (l,m).
    x2::ConstTensorAsMatrixView<double, 3> B_mat = x2::as_const_matrix(B, {0}, {1, 2});

    // C(i,j,l,m) is viewed as a matrix with rows (i,j) and columns (l,m).
    x2::X2TensorAsMatrixView<double, 4> C_mat = x2::as_matrix_x2<double, 4>(C, {0, 1}, {2, 3});

    // Compute the contraction. The output is accumulated in the high and low
    // parts of C.
    x2::gemm_x2(C_mat, A_mat, B_mat);

    Eigen::Tensor<double, 4> result = C.to_EigenTensor();

    std::println("C(0,0,0,0) = {}", result(0, 0, 0, 0));
    std::println("C(1,1,1,1) = {}", result(1, 1, 1, 1));

    // Check one entry against the explicit contraction.
    double expected = 0.0;
    for(Eigen::Index k = 0; k < A.dimension(2); ++k) expected += A(0, 0, k) * B(k, 0, 0);
    if(std::abs(result(0, 0, 0, 0) - expected) > 1e-12) return 1;

    return 0;
}
