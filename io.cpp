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
 * Copyright 2010-2018 Danny Robson <danny@nerdcruft.net>
 */

#include "io.hpp"

#include "debug.hpp"
#include "cast.hpp"
#include "format.hpp"
#include "posix/except.hpp"

#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <experimental/filesystem>
#include <stdexcept>


using namespace util;


//////////////////////////////////////////////////////////////////////////////
template <typename T>
std::vector<T>
util::slurp (const std::experimental::filesystem::path &path)
{
    static_assert (
        sizeof (T) == 1,
        "slurp is designed for grabbing bytes, not complex structures"
    );
    posix::fd out (path, O_RDONLY | O_BINARY);

    // Calculate the total file size
    off_t size = posix::error::try_value (lseek (out, 0, SEEK_END));

    posix::error::try_value (lseek (out, 0, SEEK_SET));

    // Allocate a buffer, and keep reading until it's full.
    std::vector<T> buffer (size);

    CHECK_GE (size, 0);
    size_t remaining = (size_t)size;
    T *cursor = buffer.data ();

    while (remaining) {
        ssize_t consumed = posix::error::try_value(
            ::read (out, cursor, remaining)
        );

        CHECK_GT (        consumed, 0);
        CHECK_LE ((size_t)consumed, remaining);

        remaining -= (size_t)consumed;
        cursor    += consumed;
    }

    return buffer;
}


//-----------------------------------------------------------------------------
template std::vector<char> util::slurp (const std::experimental::filesystem::path&);
template std::vector<std::byte> util::slurp (const std::experimental::filesystem::path&);


///////////////////////////////////////////////////////////////////////////////
template <typename T>
std::vector<T>
util::slurp (FILE *stream)
{
    static_assert (
        sizeof (T) == 1,
        "slurp is designed for grabbing bytes, not complex structures"
    );

    // find how much data is in this file
    const int desc = util::posix::error::try_value (fileno (stream));

    struct stat meta;
    posix::error::try_value (fstat (desc, &meta));

    std::vector<T> buf;

    // we think we know the size, so try to do a simple read
    if (meta.st_size) {
        buf.resize (meta.st_size);

        // read as much as possible, then resize to the actual length
        auto res = fread (buf.data (), 1, meta.st_size, stream);
        buf.resize (res);
    }

    // try reading small chunks until we've hit the end. important for
    // handling pipe streams (like from popen) which report a zero size.
    constexpr size_t CHUNK_SIZE = 128;
    size_t cursor = buf.size ();

    while (!feof (stream) && !ferror (stream)) {
        auto oldsize = buf.size ();
        buf.resize (oldsize + CHUNK_SIZE);
        auto res = fread (buf.data () + cursor, 1, CHUNK_SIZE, stream);

        if (res != CHUNK_SIZE)
            buf.resize (oldsize + res);
    }

    if (ferror (stream))
        throw stream_error ();

    return buf;
}


//-----------------------------------------------------------------------------
template std::vector<char> util::slurp (FILE*);
template std::vector<std::byte> util::slurp (FILE*);


///////////////////////////////////////////////////////////////////////////////
void
util::write (const posix::fd &out,
             const void *restrict data,
             size_t bytes)
{
    const char *restrict cursor = reinterpret_cast<const char*> (data);
    size_t remaining = bytes;

    while (remaining) {
        ssize_t consumed = posix::error::try_value (::write (out, cursor, remaining));

        remaining -= util::cast::sign<size_t> (consumed);
        cursor    += util::cast::sign<size_t> (consumed);
    }
}

//////////////////////////////////////////////////////////////////////////////
int
indenter::overflow (int ch) {
    if (m_line_start && ch != '\n')
        m_dest->sputn (m_indent.data (), util::cast::sign<std::streamsize> (m_indent.size ()));

    m_line_start = ch == '\n';
    return m_dest->sputc (ch);
}


//-----------------------------------------------------------------------------
indenter::indenter (std::streambuf* _dest, size_t _indent)
    : m_dest       (_dest)
    , m_line_start (true)
    , m_indent     (_indent, ' ')
    , m_owner      (NULL)
{ ; }


//-----------------------------------------------------------------------------
indenter::indenter (std::ostream& _dest, size_t _indent)
    : m_dest       (_dest.rdbuf())
    , m_line_start (true)
    , m_indent     (_indent, ' ')
    , m_owner      (&_dest)
{
    m_owner->rdbuf (this);
}


//-----------------------------------------------------------------------------
indenter::~indenter ()
{
    if (m_owner != NULL)
        m_owner->rdbuf (m_dest);
}


//////////////////////////////////////////////////////////////////////////////
scoped_cwd::scoped_cwd ()
{
    m_original.resize (16);
    while (getcwd (&m_original[0], m_original.size ()) == nullptr && errno == ERANGE)
        m_original.resize (m_original.size () * 2);
    posix::error::try_code ();
}


//-----------------------------------------------------------------------------
scoped_cwd::~scoped_cwd ()
{
    if (!chdir (m_original.c_str ()))
        posix::error::throw_code ();
}


///////////////////////////////////////////////////////////////////////////////
path_error::path_error (const std::experimental::filesystem::path &_path):
    runtime_error (to_string (format::printf ("Unknown path: %!", m_path))),
    m_path (_path)
{ ; }


//-----------------------------------------------------------------------------
const std::experimental::filesystem::path&
path_error::path (void) const noexcept
{
    return m_path;
}
