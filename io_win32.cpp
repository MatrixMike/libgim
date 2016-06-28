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
 * Copyright 2014 Danny Robson <danny@nerdcruft.net>
 */

#include "./io_win32.hpp"

#include "./debug.hpp"
#include "./except.hpp"

#include <windows.h>

using util::detail::win32::mapped_file;


///////////////////////////////////////////////////////////////////////////////
static constexpr
DWORD
fflags_to_generic (int flags) {
    switch (flags) {
    case O_RDONLY:  return GENERIC_READ;
    case O_WRONLY:  return GENERIC_WRITE;
    case O_RDWR:    return GENERIC_READ | GENERIC_WRITE;
    }

    unreachable ();
}


//-----------------------------------------------------------------------------
static constexpr
DWORD
fflags_to_access (int fflags)
{
    switch (fflags) {
    case O_RDONLY:  return FILE_MAP_READ;
    case O_WRONLY:  return FILE_MAP_WRITE;
    case O_RDWR:    return FILE_MAP_WRITE;
    }

    unreachable ();
}


//-----------------------------------------------------------------------------
static constexpr
DWORD
mflags_to_protect (int mflags) {
    DWORD res = 0;

    if (mflags & util::detail::win32::PROT_READ)  res |= PAGE_READONLY;
    if (mflags & util::detail::win32::PROT_WRITE) res |= PAGE_READWRITE;
    if (mflags & util::detail::win32::PROT_EXEC)  res |= PAGE_EXECUTE;

    return res;
}


///////////////////////////////////////////////////////////////////////////////
mapped_file::mapped_file (const boost::filesystem::path &path,
                          int fflags,
                          int mflags):
    m_data (nullptr, UnmapViewOfFile)
{
    // Cache the ASCII path to avoid memory scoping issues.
    std::string path_str = path.string ();

    // Get hold of the file we're attempting to map. Emulate some level of POXIS mmap.
    m_file.reset (
        CreateFile (
            path_str.c_str (),
            fflags_to_generic (fflags),
            fflags == O_RDONLY ? FILE_SHARE_READ : 0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
            nullptr
        )
    );

    if (m_file == INVALID_HANDLE_VALUE)
        win32_error::throw_code ();

    // I would rather perform checks on filesize after mapping, but mapping
    // requires a check for empty files before we perform the mapping to
    // detect errors it throws in that specific situation.
    DWORD hi_size, lo_size = GetFileSize (m_file, &hi_size);
    m_size = (uint64_t)hi_size << 32 | lo_size;

    m_mapping.reset (
        CreateFileMapping (
            m_file,
            nullptr,
            mflags_to_protect (mflags),
            0, 0,
            nullptr
        )
    );

    // Apparently Windows lacks the ability to map zero length files; fucking
    // hell. Try not to collapse, but instead bail with a null mapping and
    // pray the user doesn't do something stupid with the result.
    if (!m_mapping) {
        auto err = win32_error::last_code ();
        if (err == ERROR_FILE_INVALID && m_size == 0)
            return;

        win32_error::throw_code (err);
    }

    auto view = MapViewOfFile (
        m_mapping,
        fflags_to_access (fflags),
        0, 0,
        0
    );

    if (!view)
        win32_error::throw_code ();

    m_data.reset (
        static_cast<unsigned char*> (view)
    );
}


///////////////////////////////////////////////////////////////////////////////
const uint8_t*
mapped_file::data (void) const
{
    return m_data.get ();
}


//-----------------------------------------------------------------------------
uint8_t*
mapped_file::data (void)
{
    return m_data.get ();
}


//-----------------------------------------------------------------------------
size_t
mapped_file::size (void) const
{
    return m_size;
}


//-----------------------------------------------------------------------------
bool
mapped_file::empty (void) const
{
    return size () == 0;
}


//-----------------------------------------------------------------------------
uint8_t*
mapped_file::begin (void)
{
    return data ();
}


//-----------------------------------------------------------------------------
uint8_t*
mapped_file::end (void)
{
    return data () + size ();
}


//-----------------------------------------------------------------------------
const uint8_t*
mapped_file::cbegin (void) const
{
    return data ();
}


//-----------------------------------------------------------------------------
const uint8_t*
mapped_file::cend (void) const
{
    return data () + size ();
}
