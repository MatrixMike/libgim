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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */


#ifdef __UTIL_MATRIX_IPP
#error
#endif

#define __UTIL_MATRIX_IPP


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
T*
util::matrix<S,T>::operator[] (size_t idx)
{
    return this->values[idx];
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const T*
util::matrix<S,T>::operator[] (size_t idx) const
{
    return this->values[idx];
}

///////////////////////////////////////////////////////////////////////////////
//template <size_t S, typename T>
//vector<3,T>
//matrix<S,T>::operator* (vector<3,T> v) const
//{
//    return (
//        *this * v.template homog<S> ()
//    ).template redim<3> ();
//}
//
//
////-----------------------------------------------------------------------------
//template <size_t S, typename T>
//point<3,T>
//matrix<S,T>::operator* (point<3,T> p) const
//{
//    return (*this * p.template homog<S> ()).template redim<3> ();
//}
//


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
template <typename U>
util::matrix<S,U>
util::matrix<S,T>::cast (void) const
{
    util::matrix<S,U> out;

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            out.values[i][j] = values[i][j];

    return out;
}


/////////////////////////////////////////////////////////////////////////////////
//template <size_t S, typename T>
//T
//util::matrix<S,T>::determinant (void) const
//{
//    return util::determinant (*this);
//}
//
//
////-----------------------------------------------------------------------------
//template <size_t S, typename T>
//util::matrix<S,T>
//util::matrix<S,T>::inverse (void) const
//{
//    return util::inverse (*this);
//}


///////////////////////////////////////////////////////////////////////////////
//template <size_t S, typename T>
//util::matrix<S,T>
//util::matrix<S,T>::operator/ (T t) const
//{
//    auto out = *this;
//
//    for (auto &i: out.values)
//        for (auto &j: i)
//            j /= t;
//
//    return out;
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//template <size_t S, typename T>
//bool
//util::matrix<S,T>::operator== (const matrix<S,T> &m) const
//{
//    for (size_t i = 0; i < S; ++i)
//        for (size_t j = 0; j < S; ++j)
//            if (!exactly_equal (values[i][j], m[i][j]))
//                return false;
//    return true;
//}
