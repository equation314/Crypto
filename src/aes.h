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

    Aes(AESKeyLength keyLength, const ByteArray& key, AESMode mode = AES_CBC_Mode);
    virtual ~Aes();

    void setInitializationVector(const ByteArray& iv)
    {
        if (iv.length() != Nb * 4)
        {
            printf("WARNING: length of IV must be 16 bytes!\n");
            return;
        }
        m_iv = iv;
    }

    ByteArray encrypt(const ByteArray& text) const;

    ByteArray decrypt(const ByteArray& cipher) const;

private:
    static const uint8_t SBOX[0x100];
    static const uint8_t ISBOX[0x100];
    static const uint32_t RCON[10];

    int Nk, Nb, Nr, m_block_bytes;
    AESMode m_mode;
    uint32_t* m_w;
    uint32_t* m_dw;
    ByteArray m_iv;

    void fillEncTable() const;
    void fillDecTable() const;
    uint32_t invMixColumns(uint32_t x) const;
    void keyExpansion(const ByteArray& key);

    void encryptOneBlock(uint8_t* state) const;
    void decryptOneBlock(uint8_t* state) const;

    int validatePadding(const ByteArray& text) const;

    void printState(const uint8_t* state) const;
};

#endif // _AES_H
