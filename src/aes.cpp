#include "aes.h"

Aes::Aes(AESKeyLength keyLength, AESMode mode, const ByteArray& key)
{
}

Aes::~Aes()
{
}

ByteArray Aes::encode(const ByteArray& text)
{
    return text;
}

ByteArray Aes::decode(const ByteArray& cipher)
{
    return cipher;
}
