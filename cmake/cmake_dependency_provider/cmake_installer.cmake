function(x2tc_install_dependencies method package_name)
    if(NOT "${method}" STREQUAL "FIND_PACKAGE")
        return()
    endif()

    if("${package_name}" STREQUAL "Eigen3")
        if(NOT DEFINED X2TC_DEPS_BUILD_DIR)
            set(X2TC_DEPS_BUILD_DIR "${CMAKE_BINARY_DIR}/pkg-build")
        endif()
        if(NOT DEFINED X2TC_DEPS_INSTALL_DIR)
            set(X2TC_DEPS_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")
        endif()

        list(PREPEND CMAKE_PREFIX_PATH "${X2TC_DEPS_INSTALL_DIR}")
        list(REMOVE_DUPLICATES CMAKE_PREFIX_PATH)
        set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" CACHE INTERNAL "Paths for find_package config lookup" FORCE)

        find_package(Eigen3 ${ARGN} BYPASS_PROVIDER)
        if(Eigen3_FOUND)
            return()
        endif()

        set(eigen_build_dir "${X2TC_DEPS_BUILD_DIR}/Eigen3")
        execute_process(
                COMMAND "${CMAKE_COMMAND}"
                -S "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/external_Eigen3"
                -B "${eigen_build_dir}"
                -G "${CMAKE_GENERATOR}"
                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                -DCMAKE_INSTALL_PREFIX=${X2TC_DEPS_INSTALL_DIR}
                RESULT_VARIABLE eigen_configure_result)
        if(NOT eigen_configure_result EQUAL 0)
            message(FATAL_ERROR "Failed to configure Eigen3 CMake dependency install")
        endif()

        execute_process(
                COMMAND "${CMAKE_COMMAND}" --build "${eigen_build_dir}"
                RESULT_VARIABLE eigen_build_result)
        if(NOT eigen_build_result EQUAL 0)
            message(FATAL_ERROR "Failed to build/install Eigen3 CMake dependency")
        endif()

        find_package(Eigen3 ${ARGN} REQUIRED BYPASS_PROVIDER)
    endif()
endfunction()
