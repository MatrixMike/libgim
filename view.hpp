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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __UTIL_VIEW_HPP
#define __UTIL_VIEW_HPP

#include <cstdlib>
#include <iostream>

namespace util {
    class view {
    public:
        view ();
        view (const char *str);
        view (const char *first, const char *last);

        const char *begin (void) const;
        const char *end   (void) const;

        bool empty () const;
        size_t size (void) const;
        const char& operator[] (size_t) const;

        bool operator== (const char *restrict str) const;
        bool operator== (view) const;

    private:
        const char *m_begin;
        const char *m_end;
    };


    std::ostream& operator<< (std::ostream&, view);
    bool operator== (const char*, view);
}
#endif
