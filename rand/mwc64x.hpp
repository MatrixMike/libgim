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

#ifndef __UTIL_RAND_MWC64X_HPP
#define __UTIL_RAND_MWC64X_HPP

#include <cstdint>

namespace util { namespace rand {
    // multiply-with-carry style generator suitable for rapid seeking and
    // GPU generation.
    //
    // as with all such style generators, the seed value is very important.
    // don't allow either half of the uint64_t to be zero.
    struct mwc64x {
    public:
        using value_type = uint32_t;
        using seed_type  = uint64_t;

        mwc64x (seed_type);

        value_type operator() (void);

    private:
        uint64_t m_state;
    };
} }


#endif