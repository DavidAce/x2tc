#include <x2tc/x2tc.h>

int main() {
    x2::Matrix<fp64> matrix(1, 1);
    matrix.setZero();
    return matrix.size() == 1 ? 0 : 1;
}

