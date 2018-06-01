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

    ByteArray hash(const ByteArray& buffer);

private:
    static const uint64_t ROUND_CONST[24];

    int m_digest_size, m_r;
    uint64_t m_state[25];

    void keccakF1600(uint64_t* state);
};

#endif // _SHA3_H
