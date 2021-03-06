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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifdef __UTIL_REGION_IPP
#error
#endif
#define __UTIL_REGION_IPP


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
template <typename U>
constexpr util::region<S,U>
util::region<S,T>::cast (void) const
{
    return {
        p.template cast<U> (),
        e.template cast<U> ()
    };
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
constexpr
util::region<S,T>
util::region<S,T>::max (void)
{
    return {
        util::point <S,T> {std::numeric_limits<T>::lowest () / 2},
        util::extent<S,T> {std::numeric_limits<T>::max ()}
    };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
constexpr
util::region<S,T>
util::region<S,T>::unit (void)
{
    return {
        point_t::origin (),
        extent_t {1}
    };
}
