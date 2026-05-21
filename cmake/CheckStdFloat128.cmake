function(check_std_float128_t)
    include(CheckIncludeFileCXX)
    include(CheckCXXSourceCompiles)
    check_include_file_cxx(stdfloat X2TC_HAS_STDFLOAT)
    if(NOT X2TC_HAS_STDFLOAT)
        message(FATAL_ERROR "X2TC_ENABLE_128BIT requires <stdfloat>, but it was not found")
    endif()
    check_cxx_source_compiles("
        #include <stdfloat>
        int main() {
            std::float128_t x = 1.0f128;
            return x == 0;
        }" X2TC_HAS_STDFLOAT128_T)
    if(NOT X2TC_HAS_STDFLOAT128_T)
        message(FATAL_ERROR "X2TC_ENABLE_128BIT requires std::float128_t, but it was not usable")
    endif()
endfunction()

