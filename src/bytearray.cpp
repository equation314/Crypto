#include <cstring>

#include "bytearray.h"
#include "utils.h"

ByteArray::ByteArray()
    : m_len(0), m_a(nullptr)
{
}

ByteArray::ByteArray(const char* str)
    : m_len(strlen(str)), m_a(new uint8_t[m_len])
{
    memcpy(m_a, str, m_len * sizeof(uint8_t));
}

ByteArray::ByteArray(const uint8_t* array, int len)
    : m_len(len), m_a(new uint8_t[len])
{
    memcpy(m_a, array, m_len * sizeof(uint8_t));
}

ByteArray::ByteArray(const ByteArray& array)
    : m_len(array.m_len), m_a(new uint8_t[m_len])
{
    memcpy(m_a, array.m_a, m_len * sizeof(uint8_t));
}

ByteArray::ByteArray(ByteArray&& array)
    : m_len(array.m_len), m_a(array.m_a)
{
    array.m_a = nullptr;
}

ByteArray::~ByteArray()
{
    if (m_a)
        delete[] m_a;
}

ByteArray& ByteArray::operator=(ByteArray&& array)
{
    if (this != &array)
    {
        delete[] m_a;

        m_len = array.m_len;
        m_a = array.m_a;
        array.m_a = nullptr;
    }
    return *this;
}

std::string ByteArray::toPlainText() const
{
    std::string str = "";
    for (int i = 0; i < m_len; i++)
        str += char(m_a[i]);
    return str;
}

std::string ByteArray::toHexString() const
{
    std::string str = "";
    for (int i = 0; i < m_len; i++)
        str += Utils::byte2hex(m_a[i]);
    return str;
}
