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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_STREAM_HPP
#define __UTIL_STREAM_HPP

#include <iostream>
#include "types/bits.hpp"

namespace util {
    namespace stream {
        //---------------------------------------------------------------------
        template <typename T>
        struct numeric
        {
            numeric (T _val): val (_val) { ; }
            T val;
        };

        template <typename T>
        std::ostream& operator<< (std::ostream &os, numeric<T> n)
        {
            static_assert (std::is_fundamental<T>::value,
                           "numeric streamer is intended for chars");

            using integral_t = typename std::conditional<
                std::is_floating_point<T>::value,
                T,
                typename std::conditional<
                    std::is_signed<T>::value,
                    sized_type< intmax_t>::sint,
                    sized_type<uintmax_t>::uint
                >::type
            >::type;

            return os << (integral_t)n.val;
        }

        //---------------------------------------------------------------------
        class null : public std::ostream {
            public:
                std::ostream & put   (char c);
                std::ostream & write (const char *s, std::streamsize n);

                std::streampos tellp (void);
                std::ostream & seekp (std::streampos pos);
                std::ostream & seekp (std::streamoff off,
                                      std::ios_base::seekdir dir);

                std::ostream & flush (void);

                bool good (void) const;
                bool bad  (void) const;
                bool eof  (void) const;
                bool fail (void) const;

        };


        //---------------------------------------------------------------------
        struct bits {
            bits (uintmax_t value, unsigned count);

            uintmax_t value;
            unsigned  count;
        };

        std::ostream& operator<< (std::ostream&, bits);
    }
}


#endif
