#ifndef _BYTE_ARRAY_H
#define _BYTE_ARRAY_H

#include <string>

#include "utils.h"

class ByteArray
{
public:
    ByteArray();
    ByteArray(const char* str);
    ByteArray(const uint8_t* array, int len);
    ByteArray(const ByteArray& array);
    ByteArray(ByteArray&& array);
    virtual ~ByteArray();

    ByteArray& operator=(ByteArray&& array);

    int length() const { return m_len; }

    std::string toPlainText() const;

    std::string toHexString() const;

    uint8_t& operator[](int i) { return m_a[i]; }
    const uint8_t& operator[](int i) const { return m_a[i]; }

private:
    int m_len;
    uint8_t* m_a;
};

#endif // _BYTE_ARRAY_H
