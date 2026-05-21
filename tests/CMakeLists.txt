add_executable(x2tc-basic basic.cpp)
target_link_libraries(x2tc-basic PRIVATE x2tc::x2tc)
add_test(NAME x2tc-basic COMMAND x2tc-basic)

add_executable(x2tc-gemm gemm.cpp)
target_link_libraries(x2tc-gemm PRIVATE x2tc::x2tc)
add_test(NAME x2tc-gemm COMMAND x2tc-gemm)

