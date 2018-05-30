#ifndef _BYTE_ARRAY_H
#define _BYTE_ARRAY_H

#include <string>

#include "utils.h"

class ByteArray
{
public:
    ByteArray();
    ByteArray(const char* str);
    ByteArray(int len, const uint8_t* array = nullptr);
    ByteArray(const ByteArray& array);
    ByteArray(ByteArray&& array);
    virtual ~ByteArray();

    ByteArray& operator=(const ByteArray& array);
    ByteArray& operator=(ByteArray&& array);

    void clip(int size)
    {
        if (size < m_len) m_len = size;
    }

    int length() const { return m_len; }

    bool isError() const { return m_len < 0; }

    ByteArray padding(int len, uint8_t value = 0) const;

    std::string toPlainText() const;

    std::string toHexString() const;

    void saveToFile(const char* fileName) const;

    uint8_t& operator[](int i) { return m_a[i]; }
    const uint8_t& operator[](int i) const { return m_a[i]; }

    static ByteArray errorArray();
    static ByteArray fromFile(const char* fileName);
    static ByteArray fromHexString(const char* str);
    static void copy(const ByteArray& a, int sa, const ByteArray& b, int sb, int len);

private:
    int m_len;
    uint8_t* m_a;
};

#endif // _BYTE_ARRAY_H
