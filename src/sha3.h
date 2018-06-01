#ifndef _SHA3_H
#define _SHA3_H

#include "bytearray.h"

class Sha3
{
public:
    enum SHA3DigestSize
    {
        SHA3_224,
        SHA3_256,
        SHA3_384,
        SHA3_512,
    };

    Sha3(SHA3DigestSize digestSize);
    virtual ~Sha3();

    std::string algorithmName() const
    {
        char str[256];
        sprintf(str, "SHA3-%d", m_digest_size * 8);
        return str;
    }

    ByteArray hash(const ByteArray& message);

private:
    static const uint64_t ROUND_CONST[24];

    int m_digest_size, m_r;
    uint64_t m_state[25];

    void keccakF1600(uint64_t* state);
};

#endif // _SHA3_H
