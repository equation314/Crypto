#include "sha3.h"
#include "sha3const.h"

Sha3::Sha3()
{
}

Sha3::~Sha3()
{
}

void Sha3::keccak_f(uint64_t m_state[5][5])
{
    uint64_t C[5];
    uint64_t D[5];
    uint64_t B[5][5];

    for (int i = 0; i < 24; i++)
    {
        for (int x = 0; x < 5; x++)
            C[x] = m_state[x][0] ^ m_state[x][1] ^ m_state[x][2] ^ m_state[x][3] ^ m_state[x][4];

        for (int x = 0; x < 5; x++)
        {
            uint64_t tmp = C[(x + 1) % 5];
            D[x] = C[(x + 4) % 5] ^ ((tmp << 1) | (tmp >> 63));
        }

        for (int x = 0; x < 5; x++)
            for (int y = 0; y < 5; y++)
                m_state[x][y] = m_state[x][y] ^ D[x];

        for (int x = 0; x < 5; x++)
            for (int y = 0; y < 5; y++)
            {
                B[y][(2 * x + 3 * y) % 5] = m_state[x][y] << r[x][y];
                B[y][(2 * x + 3 * y) % 5] |= m_state[x][y] >> (64 - r[x][y]);
            }

        for (int x = 0; x < 5; x++)
            for (int y = 0; y < 5; y++)
                m_state[x][y] = B[x][y] ^ ((~B[(x + 1) % 5][y]) & B[(x + 2) % 5][y]);

        m_state[0][0] ^= RC[i];
    }
}

ByteArray paddingBuffer(const ByteArray& buffer)
{
    int padding = 72 - buffer.length() % 72;
    ByteArray padBuffer(buffer.length() + padding);
    ByteArray::copy(padBuffer, 0, buffer, 0, buffer.length());

    padBuffer[buffer.length()] = 0x01;
    padBuffer[padBuffer.length() - 1] ^= 0x80;
    return padBuffer;
}

ByteArray Sha3::hash(const ByteArray& buf)
{
    int32_t r = 72;
    int32_t w = 8;

    ByteArray buffer = paddingBuffer(buf);
    int size = buffer.length();

    uint64_t* ptr = (uint64_t*) &buffer[0];

    uint64_t m_state[5][5];
    memset(m_state, 0, sizeof(m_state));

    for (int i = 0; i < size / r; i++)
    {
        for (int y = 0; y < 5; y++)
            for (int x = 0; x < 5; x++)
                if ((x + 5 * y) < (r / w))
                    m_state[x][y] = m_state[x][y] ^ *(ptr + i * 9 + x + 5 * y);
        keccak_f(m_state);
    }

    ByteArray output(64);
    ptr = (uint64_t*) &output[0];

    for (int i = 0; i < 8;)
    {
        for (int y = 0; y < 5; y++)
            for (int x = 0; x < 5; x++)
                if ((x + 5 * y) < (r / w))
                    ptr[i++] ^= m_state[x][y];
    }

    return output;
}
