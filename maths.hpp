/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __MATHS_HPP
#define __MATHS_HPP

// DO NOT INCLUDE debug.hpp
// it triggers a circular dependency; debug -> format -> maths -> debug
// instead, just use cassert

#include "types/traits.hpp"
#include "float.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numeric>
#include <type_traits>


///////////////////////////////////////////////////////////////////////////////
// NOTE: You may be tempted to add all sorts of performance enhancing
// attributes (like gnu::const or gnu::pure). DO NOT DO THIS WITHOUT EXTENSIVE
// TESTING. Just about everything will break in some way with these attributes.
//
// In particular: it is safest to apply these only to leaf functions


///////////////////////////////////////////////////////////////////////////////
namespace util {
    ///////////////////////////////////////////////////////////////////////////
    // Comparisons
    inline bool
    almost_equal (const float &a, const float &b)
    {
        return ieee_single::almost_equal (a, b);
    }


    //-----------------------------------------------------------------------------
    inline bool
    almost_equal (const double &a, const double &b)
    {
        return ieee_double::almost_equal (a, b);
    }


    //-----------------------------------------------------------------------------
    template <typename A, typename B>
    inline
    typename std::enable_if_t<
        std::is_floating_point<A>::value &&
        std::is_floating_point<B>::value &&
        !std::is_same<A,B>::value ,
        bool
    >
    almost_equal (const A &a, const B &b)
    {
        using common_t = std::common_type_t<A,B>;
        return almost_equal<common_t> (static_cast<common_t> (a),
                                       static_cast<common_t> (b));
    }


    //-----------------------------------------------------------------------------
    template <typename A, typename B>
    inline
    typename std::enable_if_t<
        std::is_integral_v<A> &&
        std::is_integral_v<B> &&
        std::is_signed<A>::value == std::is_signed<B>::value,
        bool
    >
    almost_equal (const A &a, const B &b) {
        using common_t = std::common_type_t<A,B>;
        return static_cast<common_t> (a) == static_cast<common_t> (b);
    }


    //-----------------------------------------------------------------------------
    template <typename Ta, typename Tb>
    constexpr inline
    typename std::enable_if<
        !std::is_arithmetic<Ta>::value ||
        !std::is_arithmetic<Tb>::value,
        bool
    >::type
    almost_equal (const Ta &a, const Tb &b)
        { return a == b; }


    //-----------------------------------------------------------------------------
    // Useful for explictly ignore equality warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    template <typename Ta, typename Tb>
    constexpr
    typename std::enable_if_t<
        std::is_arithmetic<Ta>::value &&
        std::is_arithmetic<Tb>::value,
        bool
    >
    exactly_equal (const Ta &a, const Tb &b)
    {
        return a == b;
    }

    //-------------------------------------------------------------------------
    template <typename Ta, typename Tb>
    inline
    typename std::enable_if_t<
        !std::is_arithmetic<Ta>::value ||
        !std::is_arithmetic<Tb>::value,
        bool
    >
    exactly_equal  (const Ta &a, const Tb &b)
    {
        return a == b;
    }
#pragma GCC diagnostic pop


    //-----------------------------------------------------------------------------
    template <typename T>
    constexpr
    std::enable_if_t<
        std::is_integral<T>::value, bool
    >
    almost_zero (T t)
    {
        return t == 0;
    }

    template <typename T>
    std::enable_if_t<
        !std::is_integral<T>::value, bool
    >
    almost_zero  (T a)
        { return almost_equal (a, T{0}); }


    //-------------------------------------------------------------------------
    template <typename T>
    constexpr
    typename std::enable_if_t<
        std::is_integral<T>::value, bool
    >
    exactly_zero (T t)
    {
        return exactly_equal (t, T{0});
    }


    template <typename T>
    constexpr
    typename std::enable_if_t<
        !std::is_integral<T>::value, bool
    >
    exactly_zero (T t)
    {
        return exactly_equal (t, T{0});
    }


    //-------------------------------------------------------------------------
    template <typename T>
    constexpr
    typename std::enable_if_t<
        std::is_floating_point<T>::value, bool
    >
    is_nan (T t)
    {
        return std::isnan (t);
    }


    //-------------------------------------------------------------------------
    template <typename T>
    constexpr
    typename std::enable_if_t<
        !std::is_floating_point<T>::value, bool
    >
    is_nan (const T&)
    {
        return false;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    std::enable_if_t<std::is_arithmetic_v<T>, T>
    abs [[gnu::const]] (T t)
    {
        return t > 0 ? t : -t;
    }

    ///////////////////////////////////////////////////////////////////////////
    // exponentials

    template <typename T>
    constexpr T
    pow2  [[gnu::const]] (T value)
    {
        return value * value;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        typename BaseT,
        typename ExponentT,
        typename = std::enable_if_t<
            std::is_integral_v<ExponentT>,
            void
        >
    >
    constexpr BaseT
    pow [[gnu::const]] (BaseT base, ExponentT exponent)
    {
        assert (exponent >= 0);
        return exponent == 0 ? BaseT{1} : base * pow (base, exponent - 1);
    }


    //-------------------------------------------------------------------------
    template <typename T>
    constexpr
    std::enable_if_t<std::is_integral<T>::value, bool>
    is_pow2 [[gnu::const]]  (T value)
    {
        return value && !(value & (value - 1));
    }


    //-----------------------------------------------------------------------------
    // Logarithms
    template <typename T>
    constexpr
    std::enable_if_t<std::is_integral_v<T>, T>
    log2 (T val)
    {
        T tally = 0;
        while (val >>= 1)
            ++tally;
        return tally;
    }


    //-------------------------------------------------------------------------
    template <typename T>
    T
    log2up  (T val);


    ///////////////////////////////////////////////////////////////////////////////
    /// round T up to the nearest multiple of U
    template <typename T, typename U>
    inline
    typename std::common_type<
        std::enable_if_t<std::is_integral<T>::value,T>,
        std::enable_if_t<std::is_integral<U>::value,U>
    >::type
    round_up (T value, U size)
    {
        // we perform this as two steps to avoid unnecessarily incrementing when
        // remainder is zero.
        if (value % size)
            value += size - value % size;
        return value;
    }


    ///----------------------------------------------------------------------------
    /// round T up to the nearest power-of-2
    template <typename T>
    constexpr
    std::enable_if_t<
        std::is_integral<T>::value, T
    >
    round_pow2  (T value)
    {
        using return_type = std::enable_if_t<std::is_integral<T>::value, T>;

        --value;

        for (unsigned i = 1; i < sizeof (T) * 8; i <<= 1) {
            value |= value >> i;
        }

        ++value;
        return (return_type)value;
    }


    ///----------------------------------------------------------------------------
    /// round T up to the nearest multiple of U and return the quotient.
    template <typename T, typename U>
    constexpr std::enable_if_t<
        std::is_integral<T>::value &&
        std::is_integral<U>::value,
        T
    >
    divup  (const T a, const U b)
    {
        return (a + b - 1) / b;
    }


    ///////////////////////////////////////////////////////////////////////////////
    // Properties
    template <typename T>
    constexpr
    std::enable_if_t<std::is_integral<T>::value, bool>
    is_integer (T)
    {
        return true;
    }

    template <typename T>
    constexpr
    std::enable_if_t<std::is_floating_point<T>::value, bool>
    is_integer (T t)
    {
        T i = 0;
        return exactly_equal (std::modf (t, &i), T{0});
    }


    //-------------------------------------------------------------------------
    template <
        typename NumericT,
        typename = std::enable_if_t<
            std::is_integral_v<NumericT>,
            void
        >
    >
    constexpr auto
    digits10 (NumericT v) noexcept
    {
        // cascading conditionals are faster, but it's super annoying to write
        // out for arbitrarily sized types so we use this base case unti
        // there's actually a performance reason to use another algorithm.
        int tally = 0;
        do {
            v /= 10;
            ++tally;
        } while (v);

        return tally;

        /*
        return (v >= 1000000000) ? 10 :
               (v >=  100000000) ?  9 :
               (v >=   10000000) ?  8 :
               (v >=    1000000) ?  7 :
               (v >=     100000) ?  6 :
               (v >=      10000) ?  5 :
               (v >=       1000) ?  4 :
               (v >=        100) ?  3 :
               (v >=         10) ?  2 :
                                    1;
        */
    }


    template <typename ValueT, typename BaseT>
    constexpr
    std::enable_if_t<
        std::is_integral_v<ValueT> && std::is_integral_v<BaseT>,
        int
    >
    digits (ValueT value, BaseT base) noexcept
    {
        assert (base > 0);

        if (value < 0)
            value *= -1;

        int tally = 1;
        while (value /= base)
            ++tally;

        return tally;
    }


    ///----------------------------------------------------------------------------
    /// return positive or negative unit value corresponding to the input.
    template <typename T>
    constexpr std::enable_if_t<
        std::is_signed<T>::value && std::is_integral<T>::value, T
    >
    sign (T t)
    {
        return t < 0 ? -1 : 1;
    }

    ///------------------------------------------------------------------------
    /// return positive or negative unit value corresponding to the input.
    /// guaranteed to give correct results for signed zeroes, use another
    /// method if extreme speed is important.
    template <typename T>
    constexpr std::enable_if_t<
        std::is_floating_point<T>::value, T
    >
    sign (T t)
    {
        return std::signbit (t) ? -1 : 1;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    constexpr
    bool
    samesign (T a, T b)
    {
        return a < 0 && b < 0 || a > 0 && b > 0;
    }


    ///////////////////////////////////////////////////////////////////////////////
    // factorisation
    template <typename T>
    constexpr T
    gcd (T a, T b)
    {
        assert (a);
        assert (b);

        while (a != b) {
            if (a > b)
                a -= b;
            else if (b > a)
                b -= a;
        }

        return a;
    }


    //-----------------------------------------------------------------------------
    template <typename T>
    const T&
    identity (const T& t)
    {
        return t;
    }


    ///////////////////////////////////////////////////////////////////////////
    // Modulus/etc

    // namespaced wrapper for `man 3 fmod`
    template <typename T>
    constexpr
    std::enable_if_t<
        std::is_floating_point<T>::value, T
    >
    mod (T x, T y)
    {
        return std::fmod (x, y);
    }


    template <typename T>
    constexpr
    std::enable_if_t<
        std::is_integral<T>::value, T
    >
    mod (T x, T y)
    {
        return x % y;
    }


    ///////////////////////////////////////////////////////////////////////////////
    // angles, trig
    template <typename T>
    constexpr T PI = T(3.141592653589793238462643);


    //-----------------------------------------------------------------------------
    template <typename T>
    constexpr T E = T(2.71828182845904523536028747135266250);


    //-----------------------------------------------------------------------------
    template <typename T>
    constexpr T
    to_degrees  (T radians)
    {
        static_assert (std::is_floating_point<T>::value, "undefined for integral types");
        return radians * 180 / PI<T>;
    }


    //-----------------------------------------------------------------------------
    template <typename T>
    constexpr T
    to_radians  (T degrees)
    {
        static_assert (std::is_floating_point<T>::value, "undefined for integral types");
        return degrees / 180 * PI<T>;
    }


    //-----------------------------------------------------------------------------
    //! Normalised sinc function
    template <typename T>
    constexpr T
    sincn  (T x)
    {
        return almost_zero (x) ? 1 : std::sin (PI<T> * x) / (PI<T> * x);
    }


    //-----------------------------------------------------------------------------
    //! Unnormalised sinc function
    template <typename T>
    constexpr T
    sincu  (T x)
    {
        return almost_zero (x) ? 1 : std::sin (x) / x;
    }


    ///////////////////////////////////////////////////////////////////////////////
    // combinatorics

    constexpr uintmax_t
    factorial  (unsigned i)
    {
        return i <= 1 ? 0 : i * factorial (i - 1);
    }


    //-----------------------------------------------------------------------------
    /// stirlings approximation of factorials
    inline uintmax_t
    stirling  (unsigned n)
    {
        using real_t = double;

        return static_cast<uintmax_t> (
            std::sqrt (2 * PI<real_t> * n) * std::pow (n / E<real_t>, n)
        );
    }


    //-----------------------------------------------------------------------------
    constexpr uintmax_t
    combination  (unsigned n, unsigned k)
    {
        return factorial (n) / (factorial (k) / (factorial (n - k)));
    }


    ///////////////////////////////////////////////////////////////////////////////
    // kahan summation for long floating point sequences

    template <class InputT>
    std::enable_if_t<
        std::is_floating_point<
            typename std::iterator_traits<InputT>::value_type
        >::value,
        typename std::iterator_traits<InputT>::value_type
    >
    sum (InputT first, InputT last)
    {
        using T = typename std::iterator_traits<InputT>::value_type;

        T sum = 0;
        T c = 0;

        for (auto cursor = first; cursor != last; ++cursor) {
            // Infinities are handled poorly in this implementation. We tend
            // to produce NaNs because of the subtraction where we compute
            // `c'. For the time being just panic in this scenario.
            assert(!std::isinf (*cursor));

            T y = *cursor - c;
            T t = sum + y;

            c = (t - sum) - y;
            sum = t;
        }

        return sum;
    }


    //-------------------------------------------------------------------------
    template <class InputT>
    std::enable_if_t<
        std::is_integral<
            typename std::iterator_traits<InputT>::value_type
        >::value,
        typename std::iterator_traits<InputT>::value_type
    >
    sum (InputT first, InputT last)
    {
        using T = typename std::iterator_traits<InputT>::value_type;
        return std::accumulate (first, last, T{0});
    }


    ///////////////////////////////////////////////////////////////////////////
    /// Variadic minimum

    // disable the single parameter version for non-arithmetic types so that
    // min for coord types is unambiguous. allow pointers too because it
    // doesn't add ambiguity and it simplifies some memory juggling.
    template <
        typename T,
        typename = std::enable_if_t<
            std::is_arithmetic_v<T> || std::is_pointer_v<T>, void
        >
    >
    constexpr T
    min  (const T a)
        { return a; }


    //-------------------------------------------------------------------------
    template <typename T, typename U, typename ...Args>
    constexpr std::enable_if_t<
        std::is_unsigned<std::decay_t<T>>::value == std::is_unsigned<std::decay_t<U>>::value &&
        std::is_integral<std::decay_t<T>>::value == std::is_integral<std::decay_t<U>>::value,
        std::common_type_t<T,U>
    >
    min  (const T a, const U b, Args ...args)
    {
        return min (a < b ? a : b, std::forward<Args> (args)...);
    }


    //-------------------------------------------------------------------------
    /// Variadic maximum
    template <typename T>
    constexpr std::enable_if_t<std::is_arithmetic_v<T> || std::is_pointer_v<T>, T>
    max  (const T a)
    { return a; }


    //-------------------------------------------------------------------------
    template <typename T, typename U, typename ...Args>
    constexpr std::enable_if_t<
        std::is_unsigned<std::decay_t<T>>::value == std::is_unsigned<std::decay_t<U>>::value &&
        std::is_integral<std::decay_t<T>>::value == std::is_integral<std::decay_t<U>>::value,
        std::common_type_t<T,U>
    >
    max  (const T a, const U b, Args ...args)
    {
        return max (a > b ? a : b, std::forward<Args> (args)...);
    }


    ///////////////////////////////////////////////////////////////////////////
    // Limiting functions

    // min/max clamping
    template <typename T, typename U, typename V>
    constexpr
    std::enable_if_t<
        std::is_scalar_v<T> && std::is_scalar_v<U> && std::is_scalar_v<V>,
        std::common_type_t<T,U,V>
    >
    limit (const T val, const U lo, const V hi)
    {
        assert (lo <= hi);

        return val > hi ? hi:
               val < lo ? lo:
               val;
    }


    //-------------------------------------------------------------------------
    // clamped cubic hermite interpolation
    template <typename T>
    constexpr
    T
    smoothstep  (T a, T b, T x)
    {
        assert (a <= b);
        x = limit ((x - a) / (b - a), T{0}, T{1});
        return x * x * (3 - 2 * x);
    }


    //-------------------------------------------------------------------------
    template <typename U, typename T>
    constexpr
    std::enable_if_t<std::is_floating_point<T>::value, U>
    mix (U a, U b, T t)
    {
        assert (t >= 0);
        assert (t <= 1);

        return a * (1 - t) + b * t;
    }


    ///////////////////////////////////////////////////////////////////////////
    /// convert between different representations of normalised quantities.
    ///
    /// * floating point values must be within [0, 1] (otherwise undefined)
    /// * signed values are handled by converting to unsigned representations
    /// * may introduce small biases when expanding values so that low order
    ///   bits have some meaning (particularly when dealing with UINTMAX)

    // uint -> float
    template <typename T, typename U>
    constexpr
    typename std::enable_if<
        std::is_unsigned<T>::value && std::is_floating_point<U>::value, U
    >::type
    renormalise (T t)
    {
        return t / static_cast<U> (std::numeric_limits<T>::max ());
    }


    //-------------------------------------------------------------------------
    // float -> uint
    template <typename T, typename U>
    constexpr
    typename std::enable_if<
        std::is_floating_point<T>::value && std::is_unsigned<U>::value, U
    >::type
    renormalise (T t)
    {
        // Ideally std::ldexp would be involved but it complicates handing
        // integers with greater precision than our floating point type. Also it
        // would prohibit constexpr and involve errno.

        size_t usable    = std::numeric_limits<T>::digits;
        size_t available = sizeof (U) * 8;
        size_t shift     = std::max (available, usable) - usable;

        t = limit (t, 0, 1);

        // construct an integer of the float's mantissa size, multiply it by our
        // parameter, then shift it back into the full range of the integer type.
        U in  = std::numeric_limits<U>::max () >> shift;
        U mid = static_cast<U> (t * in);
        U out = mid << shift;

        // use the top bits of the output to fill the bottom bits which through
        // shifting would otherwise be zero. this gives us the full extent of the
        // integer range, while varying predictably through the entire output
        // space.
        return out | out >> (available - shift);
    }


    //-------------------------------------------------------------------------
    // float -> float, avoids identity conversion as we don't want to create
    // ambiguous overloads
    template <typename T, typename U>
    constexpr
    typename std::enable_if<
        std::is_floating_point<T>::value &&
        std::is_floating_point<U>::value &&
        !std::is_same<T,U>::value, U
    >::type
    renormalise (T t)
    {
        return static_cast<U> (t);
    }


    //-------------------------------------------------------------------------
    // hi_uint -> lo_uint
    template <typename T, typename U>
    constexpr
    typename std::enable_if<
        std::is_unsigned<T>::value &&
        std::is_unsigned<U>::value &&
        (sizeof (T) > sizeof (U)), U
    >::type
    renormalise (T t)
    {
        static_assert (sizeof (T) > sizeof (U),
                       "assumes right shift is sufficient");

        // we have excess bits ,just shift and return
        constexpr auto shift = 8 * (sizeof (T) - sizeof (U));
        return t >> shift;
    }


    //-------------------------------------------------------------------------
    // lo_uint -> hi_uint
    template <
        typename SrcT,
        typename DstT,
        typename = std::enable_if_t<
            std::is_unsigned<SrcT>::value &&
            std::is_unsigned<DstT>::value &&
            sizeof (SrcT) < sizeof (DstT), DstT
        >
    >
    constexpr DstT
    renormalise (SrcT src)
    {
        // we can make some simplifying assumptions for the shift distances if
        // we assume the integers are powers of two. this is probably going to
        // be the case for every conceivable input type, but we don't want to
        // get caught out if we extend this routine to more general types
        // (eg, OpenGL DEPTH24).
        static_assert (is_pow2 (sizeof (SrcT)));
        static_assert (is_pow2 (sizeof (DstT)));

        static_assert (sizeof (SrcT) < sizeof (DstT),
                       "assumes bit creation is required to fill space");

        // we need to create bits. fill the output integer with copies of ourself.
        // this is approximately correct in the general case (introducing a small
        // linear positive bias), but it allows us to set all output bits high
        // when we receive the maximum allowable input value.
        static_assert (sizeof (DstT) % sizeof (SrcT) == 0,
                       "assumes integer multiple of sizes");


        // clang#xxxx: ideally we wouldn't use a multiplication here, but we
        // trigger a segfault in clang-5.0 when using ld.gold+lto;
        // 'X86 DAG->DAG Instruction Selection'
        //
        // create a mask of locations we'd like copies of the src bit pattern.
        //
        // this replicates repeatedly or'ing and shifting dst with itself.
        DstT dst { 1 };
        for (unsigned i = sizeof (SrcT) * 8; i < sizeof (DstT) * 8; i *= 2)
            dst |= dst << i;
        return dst * src;
    }


    //-------------------------------------------------------------------------
    // identity transformation. must precede the signed cases, as they may rely
    // on this as a side effect of casts.
    template <typename T, typename U>
    constexpr
    typename std::enable_if<
        std::is_same<T,U>::value, U
    >::type
    renormalise (T t)
    { return t; }


    //-------------------------------------------------------------------------
    // anything-to-sint
    template <typename T, typename U>
    constexpr
    typename std::enable_if<
        std::is_signed<U>::value &&
        std::is_integral<U>::value &&
        !std::is_same<T,U>::value,
        U
    >::type
    renormalise (T t)
    {
        using uint_t = typename std::make_unsigned<U>::type;

        return static_cast<U> (
            ::util::renormalise<T,uint_t> (t) + std::numeric_limits<U>::min ()
        );
    };


    //-------------------------------------------------------------------------
    // sint-to-anything
    template <typename T, typename U>
    constexpr
    typename std::enable_if<
        std::is_signed<T>::value &&
        std::is_integral<T>::value &&
        !std::is_same<T,U>::value,
        U
    >::type
    renormalise (T sint)
    {
        using uint_t = typename std::make_unsigned<T>::type;

        return ::util::renormalise<uint_t,U> (
            static_cast<uint_t> (sint) - std::numeric_limits<T>::min ()
        );
    };
}


#endif // __MATHS_HPP
