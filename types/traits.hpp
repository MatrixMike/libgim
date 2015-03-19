/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_TYPES_TRAITS_HPP
#define __UTIL_TYPES_TRAITS_HPP

#include <memory>
#include <type_traits>

//-----------------------------------------------------------------------------
template <typename T> struct is_dereferencable     : std::false_type { };
template <typename T> struct is_dereferencable<T*> : std::true_type  { };
template <typename T> struct is_dereferencable<std::unique_ptr<T>> : std::true_type { };
template <typename T> struct is_dereferencable<std::shared_ptr<T>> : std::true_type { };
template <typename T> struct is_dereferencable<std::weak_ptr<T>>   : std::true_type { };
template <typename T> struct is_dereferencable<std::auto_ptr<T>>   : std::true_type { };


template <typename T> struct dereferenced_type {
    typedef typename std::enable_if<
        std::is_pointer<T>::value,
        std::remove_pointer<T>
    >::type type;
};


//-----------------------------------------------------------------------------
template <typename T> struct dereferenced_type<std::unique_ptr<T>> { typedef T type; };
template <typename T> struct dereferenced_type<std::shared_ptr<T>> { typedef T type; };
template <typename T> struct dereferenced_type<std::weak_ptr<T>>   { typedef T type; };
template <typename T> struct dereferenced_type<std::auto_ptr<T>>   { typedef T type; };


///----------------------------------------------------------------------------
/// find the unsigned version of a type if one exists
template <typename T>
struct try_unsigned
{
    typedef typename std::make_unsigned<T>::type type;
};


template <> struct try_unsigned<double> { typedef double type; };
template <> struct try_unsigned<float > { typedef float  type; };


///----------------------------------------------------------------------------
/// find the signed version of a type if one exists
template <typename T>
struct try_signed
{
    typedef typename std::make_signed<T>::type type;
};

template <> struct try_signed<double> { typedef double type; };
template <> struct try_signed<float > { typedef float  type; };

///----------------------------------------------------------------------------
/// checks if a type can be converted in all cases without modification
template <typename T, typename U> struct is_lossless_cast : std::enable_if<
    std::is_integral<T>::value &&
    std::is_integral<U>::value &&
    std::is_signed<T>::value == std::is_signed<U>::value &&
    sizeof (T) <= sizeof (U),

    std::true_type
>::value { };


//-----------------------------------------------------------------------------
template <typename T>
struct func_traits : public func_traits<decltype(&T::operator())>
{ };


template <typename C, typename R, typename ...Args>
struct func_traits<R(C::*)(Args...) const> {
    typedef R return_type;
};


template <typename R, typename ...Args>
struct func_traits<R(Args...)> {
    typedef R return_type;
};


#endif
