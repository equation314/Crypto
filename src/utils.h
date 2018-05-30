#ifndef _UTILS_H
#define _UTILS_H

#include <string>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace Utils
{
inline char hex(uint8_t bit4)
{
    return bit4 < 10 ? 48 + bit4 : 87 + bit4;
}

inline std::string byte2hex(uint8_t byte)
{
    const char str[] = {hex(byte >> 4), hex(byte & 0xf), '\0'};
    return str;
}

}; // namespace Utils

#endif // _UTILS_H
