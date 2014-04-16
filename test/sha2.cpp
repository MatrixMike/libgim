#include "../hash/sha2.hpp"

#include "../debug.hpp"

#include <cstring>

using util::hash::SHA2;

static const struct {
    const char     *input;
    SHA2::digest_t  output;
} TESTS[] = {
    { "",
      { 0xE3, 0xB0, 0xC4, 0x42, 0x98, 0xFC, 0x1C, 0x14, 0x9A, 0xFB, 0xF4, 0xC8, 0x99, 0x6F, 0xB9, 0x24,
        0x27, 0xAE, 0x41, 0xE4, 0x64, 0x9B, 0x93, 0x4C, 0xA4, 0x95, 0x99, 0x1B, 0x78, 0x52, 0xB8, 0x55 }
    },

    { "a",
      { 0xCA, 0x97, 0x81, 0x12, 0xCA, 0x1B, 0xBD, 0xCA, 0xFA, 0xC2, 0x31, 0xB3, 0x9A, 0x23, 0xDC, 0x4D,
        0xA7, 0x86, 0xEF, 0xF8, 0x14, 0x7C, 0x4E, 0x72, 0xB9, 0x80, 0x77, 0x85, 0xAF, 0xEE, 0x48, 0xBB }
    },

    { "abc",
      { 0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA, 0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23,
        0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C, 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD },
    },

    { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
      { 0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39,
        0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67, 0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1 }
    },

    { "0123456701234567012345670123456701234567012345670123456701234567",
      { 0x81, 0x82, 0xCA, 0xDB, 0x21, 0xAF, 0x0E, 0x37, 0xC0, 0x64, 0x14, 0xEC, 0xE0, 0x8E, 0x19, 0xC6,
        0x5B, 0xDB, 0x22, 0xC3, 0x96, 0xD4, 0x8B, 0xA7, 0x34, 0x10, 0x12, 0xEE, 0xA9, 0xFF, 0xDF, 0xDD }
    }
};


int
main (int, char **) {
    for (auto i: TESTS) {
        SHA2 obj;
        obj.update (reinterpret_cast<const uint8_t*> (i.input), strlen (i.input));
        obj.finish ();

        std::cout << std::hex;
        for (auto c: obj.digest ()) {
            unsigned hi = c >> 4u;
            unsigned lo = c & 0xF;

            std::cout << hi << lo << " ";
        }
        std::cout << "\n" << std::dec;

        CHECK (obj.digest () == i.output);
    }

    return 0;
}
