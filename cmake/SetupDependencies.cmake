cmake_minimum_required(VERSION 3.24)

find_package(OpenMP REQUIRED COMPONENTS CXX)
target_link_libraries(x2tc-config INTERFACE OpenMP::OpenMP_CXX)
list(APPEND X2TC_PUBLIC_DEPENDENCIES OpenMP::OpenMP_CXX)

find_package(atomic MODULE BYPASS_PROVIDER)
if(TARGET atomic::atomic)
    target_link_libraries(x2tc-config INTERFACE atomic::atomic)
    list(APPEND X2TC_PUBLIC_DEPENDENCIES atomic::atomic)
    set(X2TC_HAS_ATOMIC TRUE)
endif()

find_package(Eigen3 5)
if(NOT Eigen3_FOUND)
    find_package(Eigen3 3.4 REQUIRED)
endif()
list(APPEND X2TC_PUBLIC_DEPENDENCIES Eigen3::Eigen)
target_link_libraries(x2tc-config INTERFACE Eigen3::Eigen)

if(TARGET Eigen3::Eigen)
    set_property(TARGET Eigen3::Eigen APPEND PROPERTY INTERFACE_COMPILE_OPTIONS $<$<CXX_COMPILER_ID:GNU>:-Wno-psabi>)
endif()
