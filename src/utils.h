#ifndef _UTILS_H
#define _UTILS_H

#include <string>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace Utils
{
inline uint8_t from_hex(char hex)
{
    if ('0' <= hex && hex <= '9')
        return hex - 48;
    else if ('a' <= hex && hex <= 'f')
        return hex - 87;
    else
        return 0;
}

inline char hex(uint8_t bit4)
{
    return bit4 < 10 ? 48 + bit4 : 87 + bit4;
}

inline std::string byte2hex(uint8_t byte)
{
    const char str[] = {hex(byte >> 4), hex(byte & 0xf), '\0'};
    return str;
}

inline uint8_t hex2byte(char high, char low)
{
    return (from_hex(high) << 4) | from_hex(low);
}

inline uint8_t word2byte(uint32_t word, int n)
{
    return (word >> (n * 8)) & 0xff;
}

inline uint32_t byte2word(uint8_t b24_31, uint8_t b16_23, uint8_t b8_15, uint8_t b0_7)
{
    return (b24_31 << 24) | (b16_23 << 16) | (b8_15 << 8) | b0_7;
}

inline uint32_t rot_word(uint32_t word)
{
    return (word << 8) | (word >> 24);
}

inline uint8_t sub_byte(uint8_t byte, const uint8_t sbox[16][16])
{
    return sbox[byte >> 4][byte & 0xf];
}

inline uint32_t sub_word(uint32_t word, const uint8_t sbox[16][16])
{
    return byte2word(sub_byte(word2byte(word, 3), sbox), sub_byte(word2byte(word, 2), sbox),
                     sub_byte(word2byte(word, 1), sbox), sub_byte(word2byte(word, 0), sbox));
}

inline uint8_t xtime(uint8_t x)
{
    return (x & 0x80) ? ((x << 1) ^ 0x1b) : (x << 1);
}

}; // namespace Utils

#endif // _UTILS_H
