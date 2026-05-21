cmake_minimum_required(VERSION 3.24)
project(x2tc
        VERSION 0.1.0
        DESCRIPTION "Standalone compensated twofold arithmetic and tensor contraction library"
        HOMEPAGE_URL "https://github.com/DavidAce/x2tc"
        LANGUAGES C CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake/modules)

option(X2TC_ENABLE_TESTS "Enable unit testing with ctest" ON)
option(X2TC_CMAKE_DEBUG "Print extra CMake configuration details" OFF)

option(COMPILER_PROFILE_BUILD "Enable -ftime-trace where supported" OFF)
option(COMPILER_ENABLE_ASAN "Enable expensive runtime sanitizers" OFF)
option(COMPILER_ENABLE_USAN "Enable cheap runtime sanitizers" OFF)
option(COMPILER_ENABLE_PCH "Enable precompiled headers" OFF)
option(COMPILER_ENABLE_CCACHE "Enable ccache compiler launcher when available" OFF)
option(COMPILER_ENABLE_COVERAGE "Enable test coverage" OFF)
set(COMPILER_EXTRA_WARNINGS "" CACHE STRING "Extra C++ warning flags")

set(X2TC_PACKAGE_MANAGERS_VALID find cmake conan)
set(X2TC_PACKAGE_MANAGER find CACHE STRING "Package manager for external dependencies")
set_property(CACHE X2TC_PACKAGE_MANAGER PROPERTY STRINGS ${X2TC_PACKAGE_MANAGERS_VALID})
if(NOT X2TC_PACKAGE_MANAGER IN_LIST X2TC_PACKAGE_MANAGERS_VALID)
    message(FATAL_ERROR "X2TC_PACKAGE_MANAGER must be one of ${X2TC_PACKAGE_MANAGERS_VALID}")
endif()

add_library(x2tc-config INTERFACE)

include(cmake/SetupCcache.cmake)
include(cmake/SetupScalars.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/SetupPaths.cmake)
include(cmake/SetupDependencies.cmake)
include(cmake/GenerateInstantiationSources.cmake)

generate_scalar_instantiations(x2tc_instantiation_sources
                               TEMPLATE source/instantiations/x2tc.inst.cpp.in
                               OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated/instantiations")

add_library(x2tc-objects OBJECT ${x2tc_instantiation_sources})
target_include_directories(x2tc-objects PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(x2tc-objects PUBLIC x2tc-config)
target_link_precompiled_headers(x2tc-objects)

add_library(x2tc)
add_library(x2tc::x2tc ALIAS x2tc)
target_sources(x2tc PRIVATE $<TARGET_OBJECTS:x2tc-objects>)
target_include_directories(x2tc
                           PUBLIC
                           $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(x2tc PUBLIC cxx_std_23)
x2tc_apply_scalar_config(x2tc)
target_link_libraries(x2tc PUBLIC ${X2TC_PUBLIC_DEPENDENCIES})
target_enable_static_libgcc(x2tc)

set_target_properties(x2tc PROPERTIES
                      VERSION ${PROJECT_VERSION}
                      SOVERSION ${PROJECT_VERSION_MAJOR}
                      EXPORT_NAME x2tc)

install(TARGETS x2tc
        EXPORT x2tcTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY include/x2tc DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/x2tcConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/x2tcConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/x2tcConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/x2tc")

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/x2tcConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/x2tcConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/x2tc")

install(EXPORT x2tcTargets
        NAMESPACE x2tc::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/x2tc")

if(X2TC_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(X2TC_CMAKE_DEBUG)
    message(STATUS "x2tc package manager: ${X2TC_PACKAGE_MANAGER}")
    message(STATUS "x2tc install prefix: ${CMAKE_INSTALL_PREFIX}")
    message(STATUS "x2tc enabled scalars: ${X2TC_ENABLED_SCALARS}")
endif()
