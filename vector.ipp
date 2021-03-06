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
 * Copyright 2011-2016 Danny Robson <danny@nerdcruft.net>
 */

#if defined(__UTIL_VECTOR_IPP)
#error
#else
#define __UTIL_VECTOR_IPP
#endif

#include "maths.hpp"


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
template <size_t D>
util::vector<D,T>
util::vector<S,T>::homog (void) const
{
    static_assert (D > S, "reducing size loses data");
    return (*this).template redim<D> (0.f);
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
constexpr
util::vector<3,T>
util::cross (util::vector<3,T> a, util::vector<3,T> b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}


//-----------------------------------------------------------------------------
template <typename T>
constexpr
T
util::cross (util::vector<2,T> a, util::vector<2,T> b)
{
    return a[0] * b[1] - a[1] * b[0];
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
constexpr
util::vector<S,T>
util::vector<S,T>::ones (void)
{
    return vector<S,T> {1};
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
constexpr
util::vector<S,T>
util::vector<S,T>::zeros (void)
{
    return vector<S,T> {0};
}
