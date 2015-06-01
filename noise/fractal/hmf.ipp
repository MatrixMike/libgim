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
 * Copyright 2012-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifdef __UTIL_NOISE_FRACTAL_HMF_IPP
#error
#endif
#define __UTIL_NOISE_FRACTAL_HMF_IPP


namespace util { namespace noise { namespace fractal {
    //-------------------------------------------------------------------------
    template <typename T, typename B>
    hmf<T,B>::hmf ():
        H (0.25f),
        octaves (6),
        frequency (0.1f),
        lacunarity (2),
        offset (0.7f),
        amplitude (1),
        gain (1)
    { ; }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    constexpr T
    hmf<T,B>::operator() (util::point<2,T> p) const
    {
        T exponents[octaves];
        for (size_t i = 0; i < octaves; ++i)
            exponents[i] = std::pow (std::pow (lacunarity, float (i)), -H);

        T result = 0;
        T signal = 0;
        T weight = 1;

        p *= frequency;

        for (size_t i = 0; i < octaves; ++i) {
            signal = (basis (p) + offset) * exponents[i];
            result += weight * signal;

            weight *= gain * signal;
            if (weight > 1)
                weight = 1;

            p *= lacunarity;
        }

        return result;
    }
} } }
