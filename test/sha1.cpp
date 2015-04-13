#include "hash/sha1.hpp"

#include "debug.hpp"
#include "tap.hpp"
#include "types.hpp"

#include <cstdlib>
#include <cstring>

#include <iostream>


using util::hash::SHA1;


int
main (int, char**) {
    static const struct {
        const char *input;
        SHA1::digest_t output;
    } TESTS[] = {
        { "",
          { { 0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55,
              0xbf, 0xef, 0x95, 0x60, 0x18, 0x90, 0xaf, 0xd8, 0x07, 0x09  } }
        },

        {
          "a",
          { { 0x86, 0xf7, 0xe4, 0x37, 0xfa, 0xa5, 0xa7, 0xfc, 0xe1, 0x5d,
              0x1d, 0xdc, 0xb9, 0xea, 0xea, 0xea, 0x37, 0x76, 0x67, 0xb8  } }
        },

        { "abc",
          { { 0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A, 0xBA, 0x3E,
              0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C, 0x9C, 0xD0, 0xD8, 0x9D  } }
        },

        { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
          { { 0x84, 0x98, 0x3E, 0x44, 0x1C, 0x3B, 0xD2, 0x6E, 0xBA, 0xAE,
              0x4A, 0xA1, 0xF9, 0x51, 0x29, 0xE5, 0xE5, 0x46, 0x70, 0xF1  } }
        },

        // 1'000'000 * 'a'
        //{ "a",
        //  { { 0x34, 0xAA, 0x97, 0x3C, 0xD4, 0xC4, 0xDA, 0xA4, 0xF6, 0x1E,
        //      0xEB, 0x2B, 0xDB, 0xAD, 0x27, 0x31, 0x65, 0x34, 0x01, 0x6F  } }
        //},

        // 80 repetitions of 01234567
        //{ "0123456701234567012345670123456701234567012345670123456701234567",
        //  { { 0xDE, 0xA3, 0x56, 0xA2, 0xCD, 0xDD, 0x90, 0xC7, 0xA7, 0xEC,
        //      0xED, 0xC5, 0xEB, 0xB5, 0x63, 0x93, 0x4F, 0x46, 0x04, 0x52  } }
        //}
    };

    for (auto i: TESTS) {
        util::hash::SHA1 obj;
        obj.update (reinterpret_cast<const uint8_t*> (i.input), strlen (i.input));
        obj.finish ();

        for (uint8_t c: obj.digest ()) {
            unsigned hi = c >> 4u;
            unsigned lo = c & 0xF;

            std::cout << std::hex << hi << lo << " ";
        }
        std::cout << "\n";

        CHECK (obj.digest () == i.output);
    }

    util::TAP::logger tap;
    tap.skip ("convert to TAP");
}
