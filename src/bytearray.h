#ifndef _BYTE_ARRAY_H
#define _BYTE_ARRAY_H

#include <string>

#include "utils.h"

class ByteArray
{
public:
    ByteArray(const char* str);
    ByteArray(const uint8_t* array, int len);
    virtual ~ByteArray();

    int length() const { return m_len; }

    std::string toPlainText() const;

    std::string toHexString() const;

private:
    const uint8_t* m_a;
    int m_len;
};

#endif // _BYTE_ARRAY_H
