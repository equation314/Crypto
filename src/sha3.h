#ifndef _SHA3_H
#define _SHA3_H

#include "bytearray.h"

class Sha3
{
public:
    Sha3();
    virtual ~Sha3();

    ByteArray hash(const ByteArray& buffer);

private:
    static const uint64_t ROUND_CONST[24];

    uint64_t m_state[25];

    void keccak_f(uint64_t* state);
};

#endif // _SHA3_H
