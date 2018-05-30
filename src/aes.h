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
    static const uint8_t SBOX[16][16];
    static const uint8_t ISBOX[16][16];
    static const uint32_t RCON[10];

    int Nk, Nb, Nr, m_block_bytes;
    AESMode m_mode;
    uint32_t* m_w;
    ByteArray m_iv;

    void encryptOneBlock(uint8_t* state) const;
    void decryptOneBlock(uint8_t* state) const;

    int validatePadding(const ByteArray& text) const;

    void keyExpansion(const ByteArray& key);

    void addRoundKey(uint8_t* state, const uint32_t* roundKey) const;

    void subBytes(uint8_t* state) const;
    void shiftRows(uint8_t* state) const;
    void mixColumns(uint8_t* state) const;

    void invSubBytes(uint8_t* state) const;
    void invShiftRows(uint8_t* state) const;
    void invMixColumns(uint8_t* state) const;

    void printState(const uint8_t* state) const;
};

#endif // _AES_H
