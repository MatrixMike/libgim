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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */

#ifdef   __UTIL_IO_POSIX_IPP
#error
#endif

#define __UTIL_IO_POSIX_IPP


///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::detail::posix::mapped_file::operator util::view<const T *restrict> () const &
{
    return {
        reinterpret_cast<const T*restrict> (cbegin ()),
        reinterpret_cast<const T*restrict> (cend ())
    };
}


//-----------------------------------------------------------------------------
template <typename T>
util::detail::posix::mapped_file::operator util::view<T *restrict> () &
{
    return {
        reinterpret_cast<T* restrict> (begin ()),
        reinterpret_cast<T* restrict> (end ())
    };
}
