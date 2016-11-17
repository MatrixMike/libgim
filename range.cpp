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
 * Copyright 2011-2012 Danny Robson <danny@nerdcruft.net>
 */


#include "range.hpp"

#include "debug.hpp"
#include "json/tree.hpp"
#include "maths.hpp"

#include <limits>
#include <cmath>
#include <cstdint>


///////////////////////////////////////////////////////////////////////////////
template <typename T>
const util::range<T>
util::range<T>::UNLIMITED (std::numeric_limits<T>::has_infinity ? -std::numeric_limits<T>::infinity () :
                                                                   std::numeric_limits<T>::lowest   (),
                           std::numeric_limits<T>::has_infinity ?  std::numeric_limits<T>::infinity () :
                                                              std::numeric_limits<T>::max      ());

//-----------------------------------------------------------------------------
template <typename T>
const util::range<T>
util::range<T>::MAX (std::numeric_limits<T>::lowest (),
                     std::numeric_limits<T>::max ());


//-----------------------------------------------------------------------------
template <typename T>
const util::range<T>
util::range<T>::UNIT (0.0, 1.0);


///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::range<T>::range (T _min, T _max):
        min (_min),
        max (_max)
{
    sanity ();
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
T
util::range<T>::magnitude (void) const
{
    return max - min;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool
util::range<T>::contains (T val) const
{
    return val >= min && val <= max;
}


//-----------------------------------------------------------------------------
template <typename T>
bool
util::range<T>::contains (const range <T> &r) const
{
    return r.min >= min && r.max <= max;
}


//-----------------------------------------------------------------------------
template <typename T>
T
util::range<T>::at (float t) const
{
    return static_cast<T> (min + (max - min) * t);
}


//-----------------------------------------------------------------------------
template <typename T>
T
util::range<T>::clamp (T val) const
{
    return std::max (min, std::min (val, max));
}


//-----------------------------------------------------------------------------
template <typename T>
void
util::range<T>::expand (T val)
{
    // The arguments to min and max are such that expansion from initial NaN
    // values should change both min and max to be that value.
    min = std::min (val, min);
    max = std::max (val, max);
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::range<T>&
util::range<T>::operator*= (T val)
{
    min *= val;
    max *= val;

    return *this;
}


//-----------------------------------------------------------------------------
template <typename T>
util::range<T>
util::range<T>::operator* (T val) const
{
    return util::range<T> (min * val, max * val);
}


//-----------------------------------------------------------------------------
template <typename T>
util::range<T>
util::range<T>::operator- (T val) const
{
    return util::range<T> (min - val, max - val);
}


///////////////////////////////////////////////////////////////////////////////
namespace util {
    template <>
    double
    range<double>::random (void) const
    {
        double pos = ::rand () / (double)(RAND_MAX);
        return (max - min) * pos + min;
    }

    template <>
    float
    range<float>::random (void) const
    {
        float pos = ::rand () / (float)(RAND_MAX);
        return (max - min) * pos + min;
    }
}


//-----------------------------------------------------------------------------
template <typename T>
T
util::range<T>::random (void) const
{
    return min + (T)::rand () % (max - min);
}


///////////////////////////////////////////////////////////////////////////////
namespace util {
    template <>
    bool
    range<float>::operator ==(const range<float> &rhs) const
    {
        return almost_equal (min, rhs.min) &&
               almost_equal (max, rhs.max);
    }


    template <>
    bool
    range<double>::operator ==(const range<double> &rhs) const
    {
        return almost_equal (min, rhs.min) &&
               almost_equal (max, rhs.max);
    }
}


//-----------------------------------------------------------------------------
template <typename T>
bool
util::range<T>::operator ==(const util::range<T> &rhs) const
{
    return min == rhs.min && max == rhs.max;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
void
util::range<T>::sanity (void) const
{
    CHECK (min <= max);
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    void
    range<double>::sanity (void) const
    {
        if (std::isnan (min) || std::isnan (max))
            return;
        CHECK (min <= max);
    }
}


//-----------------------------------------------------------------------------
namespace util {
    template struct range<double>;
    template struct range<float>;
    template struct range<uint8_t>;
    template struct range<uint16_t>;
    template struct range<uint32_t>;
    template struct range<uint64_t>;
}


//-----------------------------------------------------------------------------
namespace json { namespace tree {
    template <>
    util::range<double>
    io<util::range<double>>::deserialise (const json::tree::node &node)
    {
        if (node.is_string () && (node == "UNIT" ||
                                  node == "unit")) {
            return util::range<double>::UNIT;
        } else if (node.is_string () && (node == "UNLIMITED" ||
                                         node == "unlimited")) {
            return util::range<double>::UNLIMITED;
        } else {
            return {
                node[0].as_number (),
                node[1].as_number ()
            };
        }
    }
} }
