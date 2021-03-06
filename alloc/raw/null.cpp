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


#include "null.hpp"

#include "../../debug.hpp"

#include <new>

using util::alloc::raw::null;


///////////////////////////////////////////////////////////////////////////////
void*
null::allocate (size_t bytes)
{
    return allocate (bytes, alignof (std::max_align_t));
}


//-----------------------------------------------------------------------------
void*
null::allocate (size_t bytes, size_t align)
{
    (void)bytes;
    (void)align;

    throw std::bad_alloc ();
}


//-----------------------------------------------------------------------------
void
null::deallocate (void *ptr, size_t bytes)
{
    return deallocate (ptr, bytes, alignof (std::max_align_t));
}


//-----------------------------------------------------------------------------
// calling deallocate with a non-null pointer is undefined, but we may as well
// let the application continuing running if we're not in a debug context.
void
null::deallocate (void *ptr, size_t bytes, size_t align)
{
    (void)ptr;
    (void)bytes;
    (void)align;

    // cast to void* to assist some of the printing machinary in the assertion
    CHECK_EQ (ptr, (void*)nullptr);
}


///////////////////////////////////////////////////////////////////////////////
void*
null::begin (void)
{
    return nullptr;
}


//-----------------------------------------------------------------------------
const void*
null::begin (void) const
{
    return nullptr;
}


//-----------------------------------------------------------------------------
size_t
null::offset (const void *ptr) const
{
    return reinterpret_cast<uintptr_t> (ptr);
}


///////////////////////////////////////////////////////////////////////////////
void
null::reset (void)
{
    ;
}


///////////////////////////////////////////////////////////////////////////////
size_t
null::capacity (void) const
{
    return 0;
}


//-----------------------------------------------------------------------------
size_t
null::used (void) const
{
    return 0;
}


//-----------------------------------------------------------------------------
size_t
null::remain (void) const
{
    return 0;
}
