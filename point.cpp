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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#include "point.hpp"

#include "debug.hpp"
#include "maths.hpp"

#include <cmath>
#include <cstdlib>

using namespace std;


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
util::vector<S,T>
util::point<S,T>::to (const point<S,T> &rhs) const
{
    util::vector<S,T> out;

    for (size_t i = 0; i < S; ++i)
        out.data[i] = rhs.data[i] - this->data[i];
    return out;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::vector<S,T>
util::point<S,T>::from (const point<S,T> &rhs) const
{
    util::vector<S,T> out;
    for (size_t i = 0; i < S; ++i)
        out.data[i] = this->data[i] - rhs.data[i];
    return out;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
void
util::point<S,T>::sanity (void) const {
    CHECK (std::all_of (begin (this->data),
                        end   (this->data),
                        [] (double i) { return !std::isnan (i); }));
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const util::point<S,T> util::point<S,T>::ORIGIN (T {0});


//-----------------------------------------------------------------------------
template <size_t S, typename T>
std::ostream&
util::operator<< (std::ostream &os, const util::point<S,T> &p) {
    os << "point" << S << "(";
    os << p.data[0];

    for (size_t i = 1; i < S; ++i)
        os << ", " << p.data[i];

    os << ")";
    return os;
}


//-----------------------------------------------------------------------------
#define INSTANTIATE_S_T(S,T)                                                            \
template struct util::point<S,T>;                                                       \
template std::ostream& util::operator<< (std::ostream &os, const util::point<S,T>&);    \

#define INSTANTIATE(T)  \
INSTANTIATE_S_T(1,T)    \
INSTANTIATE_S_T(2,T)    \
INSTANTIATE_S_T(3,T)    \
INSTANTIATE_S_T(4,T)


INSTANTIATE(int32_t)
INSTANTIATE(uint32_t)
INSTANTIATE(int64_t)
INSTANTIATE(uint64_t)
INSTANTIATE(float)
INSTANTIATE(double)
