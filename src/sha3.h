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
    // uint64_t m_state[5][5];

    void keccak_f(uint64_t A[5][5]);
};

#endif // _SHA3_H
