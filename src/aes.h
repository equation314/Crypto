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
    static const uint8_t SBOX[16][16];
    static const uint8_t ISBOX[16][16];
    static const uint32_t RCON[10];

    int Nk, Nb, Nr;
    AESMode m_mode;
    uint32_t* m_w;

    void keyExpansion(const ByteArray& key);
};

#endif // _AES_H
