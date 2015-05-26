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

#ifndef __UTIL_HASH_MURMUR_MURMUR2_HPP
#define __UTIL_HASH_MURMUR_MURMUR2_HPP

#include <cstddef>
#include <cstdint>

// Austin Appleby's MumurHash2, and MurmurHash64A. The exhaustive list of
// variants is deliberately not provided. You can damn well align your data or
// fix the algorithm.
namespace util { namespace hash { namespace murmur2 {
    uint32_t mix (uint32_t, uint32_t);
    uint64_t mix (uint64_t, uint64_t);

    uint32_t hash_32 (const void *restrict data, size_t len, uint32_t seed);
    uint64_t hash_64 (const void *restrict data, size_t len, uint64_t seed);
} } }

#endif
