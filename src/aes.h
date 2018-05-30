#ifndef _AES_H
#define _AES_H

#include "bytearray.h"

class Aes
{
public:
    enum AESKeyLength
    {
        AES_128,
        AES_192,
        AES_256,
    };

    enum AESMode
    {
        AES_ECB_Mode,
        AES_CBC_Mode,
        AES_CFB_Mode,
        AES_OFB_Mode,
    };

    Aes(AESKeyLength keyLength, AESMode mode, const ByteArray& key);
    virtual ~Aes();

    ByteArray encode(const ByteArray& text);

    ByteArray decode(const ByteArray& cipher);

private:
};

#endif // _AES_H
