#include "hash/md2.hpp"

#include "tap.hpp"

using util::hash::MD2;

#include <cstring>
#include <iostream>


int
main (int, char **) {
    static const struct {
        const char    *input;
        MD2::digest_t  output;
    } TESTS[] = {
        { "",
          { { 0x83, 0x50, 0xe5, 0xa3, 0xe2, 0x4c, 0x15, 0x3d,
              0xf2, 0x27, 0x5c, 0x9f, 0x80, 0x69, 0x27, 0x73 } }
        },
        { "a",
          { { 0x32, 0xec, 0x01, 0xec, 0x4a, 0x6d, 0xac, 0x72,
              0xc0, 0xab, 0x96, 0xfb, 0x34, 0xc0, 0xb5, 0xd1 } }
        },
        { "abc",
          { { 0xda, 0x85, 0x3b, 0x0d, 0x3f, 0x88, 0xd9, 0x9b,
              0x30, 0x28, 0x3a, 0x69, 0xe6, 0xde, 0xd6, 0xbb } }
        },
        { "message digest",
          { { 0xab, 0x4f, 0x49, 0x6b, 0xfb, 0x2a, 0x53, 0x0b,
              0x21, 0x9f, 0xf3, 0x30, 0x31, 0xfe, 0x06, 0xb0 } }
        },
        { "abcdefghijklmnopqrstuvwxyz",
          { { 0x4e, 0x8d, 0xdf, 0xf3, 0x65, 0x02, 0x92, 0xab,
              0x5a, 0x41, 0x08, 0xc3, 0xaa, 0x47, 0x94, 0x0b } }
        },
        { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
          { { 0xda, 0x33, 0xde, 0xf2, 0xa4, 0x2d, 0xf1, 0x39,
              0x75, 0x35, 0x28, 0x46, 0xc3, 0x03, 0x38, 0xcd } }
        },
        { "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
          { { 0xd5, 0x97, 0x6f, 0x79, 0xd8, 0x3d, 0x3a, 0x0d,
              0xc9, 0x80, 0x6c, 0x3c, 0x66, 0xf3, 0xef, 0xd8 } }
        }
    };

    bool success = true;

    for (const auto &i: TESTS) {
        MD2 h;
        h.update (i.input, strlen (i.input));
        h.finish ();
        auto out = h.digest ();

        if (out != i.output) {
            std::cerr << "Failed on " << i.input << "\n";
            success = false;
        }
    }

    util::TAP::logger tap;
    tap.expect (success, "test vectors");
    return tap.status ();
}