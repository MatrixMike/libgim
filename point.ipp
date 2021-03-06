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
 * Copyright 2014-2015 Danny Robson <danny@nerdcruft.net>
 */

#include "maths.hpp"

#include <algorithm>


///////////////////////////////////////////////////////////////////////////////
/// expand point to use homogenous coordinates of a higher dimension.
/// ie, fill with (0,..,0,1)
template <size_t S, typename T>
template <size_t D>
util::point<D,T>
util::point<S,T>::homog (void) const
{
    static_assert (D > S, "homog will not overwrite data");

    point<D,T> out;

    // Copy the existing data
    auto c = std::copy (this->begin (),
                        this->end (),
                        out.begin ());

    // Fill until the second last element with zeros
    auto f = std::fill_n (c, D - S - 1, T{0});

    // Last element should be one
    *f = T{1};

    return out;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
constexpr
util::point<S,T>
util::point<S,T>::origin (void)
{
    return point<S,T> {0};
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T, typename U>
typename std::common_type<T,U>::type
util::distance (point<S,T> a, point<S,U> b)
{
    using type_t = typename std::common_type<T,U>::type;
    static_assert (std::is_floating_point<type_t>::value,
                   "sqrt likely requires fractional types");

    return std::sqrt (distance2 (a, b));
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T, typename U>
constexpr typename std::common_type<T,U>::type
util::distance2 (point<S,T> a, point<S,U> b)
{
    return sum (pow2 (a - b));
}


///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U>
typename std::common_type<T,U>::type
util::octile (point2<T> a, point2<U> b)
{
    using type_t = typename std::common_type<T,U>::type;
    static_assert (!std::is_integral<type_t>::value,
                   "octile requires more than integer precision");

    const type_t D1 = 1;
    const type_t D2 = std::sqrt (type_t {2});

    auto diff = util::abs (a - b);

    // distance for axis-aligned walks
    auto axis = D1 * (diff.x + diff.y);

    // the savings from diagonal walks
    auto diag = (D2 - 2 * D1) * util::min (diff);

    return axis + diag;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T, typename U>
constexpr typename std::common_type<T,U>::type
util::manhattan (point<S,T> a, point<S,U> b)
{
    return sum (abs (a - b));
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T, typename U>
constexpr typename std::common_type<T,U>::type
util::chebyshev(point<S,T> a, point<S,U> b)
{
    return util::max (abs (a - b));
}
