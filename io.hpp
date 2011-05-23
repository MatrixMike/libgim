/*
 * This file is part of waif.
 *
 * Waif is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Waif is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with waif.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2010 Danny Robson <danny@blubinc.net>
 */

#ifndef __UTIL_IO_HPP
#define __UTIL_IO_HPP

#include <cstdio>
#include <cstdint>
#include <memory>
#include <boost/filesystem.hpp>


enum access_t {
    ACCESS_READ      = 1 << 0,
    ACCESS_WRITE     = 1 << 1,
    ACCESS_READWRITE = ACCESS_READ | ACCESS_WRITE
};


struct FILE_destructor {
    void operator ()(FILE *f) { fclose (f); }
};

typedef std::unique_ptr <FILE, FILE_destructor> FILE_ref;


struct fd_destructor {
    void operator ()(int fd) { close (fd); }
};

struct fd_ref {
    public:
        int m_fd;


        fd_ref  (int _fd);
        ~fd_ref ();

        operator int (void) const;
};


class mapped_file {
    protected:
        fd_ref   m_fd;
        uint8_t *m_data;
        size_t   m_size;

        void load_fd (void);

    public:
        mapped_file (const char                    *path);
        mapped_file (const std::string             &path);
        mapped_file (const boost::filesystem::path &path);

        mapped_file (const mapped_file             &rhs);
        mapped_file& operator =(const mapped_file  &rhs);

        ~mapped_file ();

        const uint8_t* data (void) const;
        size_t         size (void) const;
};


#endif
