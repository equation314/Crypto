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

ByteArray::ByteArray(int len, const uint8_t* array)
    : m_len(len), m_a(new uint8_t[len])
{
    if (array)
        memcpy(m_a, array, m_len * sizeof(uint8_t));
    else
        memset(m_a, 0, m_len * sizeof(uint8_t));
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

ByteArray& ByteArray::operator=(const ByteArray& array)
{
    m_len = array.m_len;
    memcpy(m_a, array.m_a, m_len * sizeof(uint8_t));
    return *this;
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

ByteArray ByteArray::padding(uint8_t align, uint8_t value) const
{
    int pad = align - m_len % align;
    ByteArray array(m_len + pad);
    memcpy(array.m_a, m_a, m_len * sizeof(uint8_t));
    memset(array.m_a + m_len, value, pad * sizeof(uint8_t));
    return array;
}

ByteArray ByteArray::padding101(uint8_t align) const
{
    int pad = align - m_len % align;
    ByteArray array(m_len + pad);
    memcpy(array.m_a, m_a, m_len * sizeof(uint8_t));
    array[m_len] = 0x01;
    array[array.m_len - 1] ^= 0x80;
    return array;
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

void ByteArray::saveToFile(const char* fileName) const
{
    FILE* file = fopen(fileName, "wb");

    const int BUF_SIZE = 1 << 20;
    for (int i = 0; i < m_len; i += BUF_SIZE)
        fwrite(m_a + i, sizeof(uint8_t), std::min(BUF_SIZE, m_len - i), file);

    fclose(file);
}

bool ByteArray::operator==(const ByteArray& array) const
{
    if (m_len != array.m_len)
        return false;
    for (int i = 0; i < m_len; i++)
        if (m_a[i] != array[i])
            return false;
    return true;
}

ByteArray ByteArray::errorArray()
{
    ByteArray array;
    array.m_len = -1;
    return array;
}

ByteArray ByteArray::fromFile(const char* fileName)
{
    FILE* file = fopen(fileName, "rb");
    if (file == NULL)
        return ByteArray();

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    const int BUF_SIZE = 1 << 20;
    ByteArray array(size);
    for (int i = 0; i < size; i += BUF_SIZE)
        fread(array.m_a + i, sizeof(uint8_t), BUF_SIZE, file);
    fclose(file);

    return array;
}

ByteArray ByteArray::fromHexString(const char* str)
{
    int len = strlen(str) / 2;
    ByteArray array(len);
    for (int i = 0; i < len; i++)
        array[i] = Utils::hex2byte(str[i * 2], str[i * 2 + 1]);
    return array;
}

void ByteArray::copy(const ByteArray& a, int sa, const ByteArray& b, int sb, int len)
{
    memcpy(a.m_a + sa, b.m_a + sb, len * sizeof(uint8_t));
}
