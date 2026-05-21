#pragma once

#include <x2tc/internal/scalars.h>
#include <Eigen/Core>
#include <climits>

namespace Eigen {
#if defined(X2TC_USE_FLOAT128)
    template<>
    struct NumTraits<fp128> : NumTraits<double> {
        typedef fp128 Real;
        typedef fp128 NonInteger;
        typedef fp128 Nested;

        enum {
            IsComplex = 0, IsInteger = 0, IsSigned = 1, RequireInitialization = 1, ReadCost = 1, AddCost = 3,
            MulCost = 3
        };
    };

    template<>
    struct NumTraits<cx128> : NumTraits<cx64> {
        typedef fp128 Real;
        typedef fp128 NonInteger;
        typedef fp128 Nested;

        enum {
            IsComplex = 1, IsInteger = 0, IsSigned = 1, RequireInitialization = 1, ReadCost = 1, AddCost = 6,
            MulCost = 6
        };
    };

    namespace numext {
        template<size_t Size>
        struct get_integer_by_size;

        template<>
        struct get_integer_by_size<16> {
            typedef __int128_t signed_type;
            typedef __uint128_t unsigned_type;
        };
    }

    namespace internal {
        template<typename T>
        struct is_arithmetic;

        template<>
        struct is_arithmetic<fp128> {
            enum { value = true };
        };
    }
#endif
}
