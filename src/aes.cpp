#include <assert.h>

#include "aes.h"
#include "aesconst.h"

static uint32_t Te[0x100 * 4];
static uint32_t Td[0x100 * 4];

Aes::Aes(AESKeyLength keyLength, const ByteArray& key, AESMode mode)
    : m_mode(mode), m_w(nullptr), m_dw(nullptr), m_iv(16)
{
    switch (keyLength)
    {
    default:
    case AES_128:
        Nk = 4, Nb = 4, Nr = 10;
        break;
    case AES_192:
        Nk = 6, Nb = 4, Nr = 12;
        break;
    case AES_256:
        Nk = 8, Nb = 4, Nr = 14;
        break;
        break;
    }
    m_block_bytes = Nb * 4;
    fillEncTable();
    fillDecTable();
    keyExpansion(key.length() >= Nk * 4 ? key : key.padding(Nk * 4));
}

Aes::~Aes()
{
    if (m_w)
        delete[] m_w;
    if (m_dw)
        delete[] m_dw;
}

ByteArray Aes::encrypt(const ByteArray& text) const
{
    int padding = m_block_bytes - text.length() % m_block_bytes;
    ByteArray state(text.length() + padding);
    ByteArray::copy(state, 0, text, 0, text.length());
    for (int i = text.length(); i < state.length(); i++)
        state[i] = padding;

    const uint8_t* last = &m_iv[0];
    switch (m_mode)
    {
    default:
    case AES_CBC_Mode:
        for (int i = 0; i < state.length(); i += m_block_bytes)
        {
            for (int j = 0; j < m_block_bytes; j += 4)
                *(uint32_t*) (&state[i + j]) ^= *(uint32_t*) (&last[j]);
            encryptOneBlock(&state[i]);
            last = &state[i];
        }
        break;
    case AES_ECB_Mode:
        for (int i = 0; i < state.length(); i += m_block_bytes)
            encryptOneBlock(&state[i]);
        break;
    }

    return state;
}

ByteArray Aes::decrypt(const ByteArray& cipher) const
{
    assert(cipher.length() % m_block_bytes == 0);

    const uint8_t* last = &m_iv[0];
    ByteArray state(cipher);
    switch (m_mode)
    {
    default:
    case AES_CBC_Mode:
        for (int i = 0; i < cipher.length(); i += m_block_bytes)
        {
            decryptOneBlock(&state[i]);
            for (int j = 0; j < m_block_bytes; j += 4)
                *(uint32_t*) (&state[i + j]) ^= *(uint32_t*) (&last[j]);
            last = &cipher[i];
        }
        break;
    case AES_ECB_Mode:
        for (int i = 0; i < cipher.length(); i += m_block_bytes)
            decryptOneBlock(&state[i]);
        break;
    }

    int padding = validatePadding(state);
    if (padding < 0)
        return ByteArray::errorArray();
    else
        state.clip(state.length() - padding);

    return state;
}

int Aes::validatePadding(const ByteArray& text) const
{
    int padding = text[text.length() - 1];
    if (padding == 0 || padding > 16 || padding > text.length()) return -1;
    for (int i = 1; i <= padding; i++)
        if (text[text.length() - i] != padding) return -1;
    return padding;
}

void Aes::encryptOneBlock(uint8_t* state) const
{
#define QUARTER_ROUND_E(t, a, b, c, d) \
    d ^= Te[(uint8_t) t + 3 * 256];     \
    t >>= 8;                            \
    c ^= Te[(uint8_t) t + 2 * 256];     \
    t >>= 8;                            \
    b ^= Te[(uint8_t) t + 1 * 256];     \
    t >>= 8;                            \
    a ^= Te[(uint8_t) t + 0 * 256]

#define QUARTER_ROUND_E2(state, t, a, b, c, d) \
    state[d] = SBOX[(uint8_t) t];               \
    t >>= 8;                                    \
    state[c] = SBOX[(uint8_t) t];               \
    t >>= 8;                                    \
    state[b] = SBOX[(uint8_t) t];               \
    t >>= 8;                                    \
    state[a] = SBOX[(uint8_t) t]

    const uint32_t* rk = m_w;

    uint32_t s0 = *(uint32_t*) (&state[0]) ^ rk[0];
    uint32_t s1 = *(uint32_t*) (&state[4]) ^ rk[1];
    uint32_t s2 = *(uint32_t*) (&state[8]) ^ rk[2];
    uint32_t s3 = *(uint32_t*) (&state[12]) ^ rk[3];
    uint32_t t0 = rk[4];
    uint32_t t1 = rk[5];
    uint32_t t2 = rk[6];
    uint32_t t3 = rk[7];
    rk += 8;

    QUARTER_ROUND_E(s3, t0, t1, t2, t3);
    QUARTER_ROUND_E(s2, t3, t0, t1, t2);
    QUARTER_ROUND_E(s1, t2, t3, t0, t1);
    QUARTER_ROUND_E(s0, t1, t2, t3, t0);

    int r = Nr / 2 - 1;
    while (r > 0)
    {
        s0 = rk[0], s1 = rk[1], s2 = rk[2], s3 = rk[3];
        QUARTER_ROUND_E(t3, s0, s1, s2, s3);
        QUARTER_ROUND_E(t2, s3, s0, s1, s2);
        QUARTER_ROUND_E(t1, s2, s3, s0, s1);
        QUARTER_ROUND_E(t0, s1, s2, s3, s0);

        t0 = rk[4], t1 = rk[5], t2 = rk[6], t3 = rk[7];
        QUARTER_ROUND_E(s3, t0, t1, t2, t3);
        QUARTER_ROUND_E(s2, t3, t0, t1, t2);
        QUARTER_ROUND_E(s1, t2, t3, t0, t1);
        QUARTER_ROUND_E(s0, t1, t2, t3, t0);

        rk += 8, r--;
    }

    QUARTER_ROUND_E2(state, t3, 3, 6, 9, 12);
    QUARTER_ROUND_E2(state, t2, 15, 2, 5, 8);
    QUARTER_ROUND_E2(state, t1, 11, 14, 1, 4);
    QUARTER_ROUND_E2(state, t0, 7, 10, 13, 0);

    *(uint32_t*) (&state[0]) ^= rk[0];
    *(uint32_t*) (&state[4]) ^= rk[1];
    *(uint32_t*) (&state[8]) ^= rk[2];
    *(uint32_t*) (&state[12]) ^= rk[3];
}

void Aes::decryptOneBlock(uint8_t* state) const
{
#define QUARTER_ROUND_D(t, a, b, c, d) \
    d ^= Td[(uint8_t) t + 3 * 256];     \
    t >>= 8;                            \
    c ^= Td[(uint8_t) t + 2 * 256];     \
    t >>= 8;                            \
    b ^= Td[(uint8_t) t + 1 * 256];     \
    t >>= 8;                            \
    a ^= Td[(uint8_t) t + 0 * 256]

#define QUARTER_ROUND_D2(state, t, a, b, c, d) \
    state[d] = ISBOX[(uint8_t) t];              \
    t >>= 8;                                    \
    state[c] = ISBOX[(uint8_t) t];              \
    t >>= 8;                                    \
    state[b] = ISBOX[(uint8_t) t];              \
    t >>= 8;                                    \
    state[a] = ISBOX[(uint8_t) t]

    const uint32_t* rk = m_dw;

    uint32_t s0 = *(uint32_t*) (&state[0]) ^ rk[0];
    uint32_t s1 = *(uint32_t*) (&state[4]) ^ rk[1];
    uint32_t s2 = *(uint32_t*) (&state[8]) ^ rk[2];
    uint32_t s3 = *(uint32_t*) (&state[12]) ^ rk[3];
    uint32_t t0 = rk[4];
    uint32_t t1 = rk[5];
    uint32_t t2 = rk[6];
    uint32_t t3 = rk[7];
    rk += 8;

    QUARTER_ROUND_D(s3, t2, t1, t0, t3);
    QUARTER_ROUND_D(s2, t1, t0, t3, t2);
    QUARTER_ROUND_D(s1, t0, t3, t2, t1);
    QUARTER_ROUND_D(s0, t3, t2, t1, t0);

    int r = Nr / 2 - 1;
    while (r > 0)
    {
        s0 = rk[0], s1 = rk[1], s2 = rk[2], s3 = rk[3];
        QUARTER_ROUND_D(t3, s2, s1, s0, s3);
        QUARTER_ROUND_D(t2, s1, s0, s3, s2);
        QUARTER_ROUND_D(t1, s0, s3, s2, s1);
        QUARTER_ROUND_D(t0, s3, s2, s1, s0);

        t0 = rk[4], t1 = rk[5], t2 = rk[6], t3 = rk[7];
        QUARTER_ROUND_D(s3, t2, t1, t0, t3);
        QUARTER_ROUND_D(s2, t1, t0, t3, t2);
        QUARTER_ROUND_D(s1, t0, t3, t2, t1);
        QUARTER_ROUND_D(s0, t3, t2, t1, t0);

        rk += 8, r--;
    }

    QUARTER_ROUND_D2(state, t3, 11, 6, 1, 12);
    QUARTER_ROUND_D2(state, t2, 7, 2, 13, 8);
    QUARTER_ROUND_D2(state, t1, 3, 14, 9, 4);
    QUARTER_ROUND_D2(state, t0, 15, 10, 5, 0);

    *(uint32_t*) (&state[0]) ^= rk[0];
    *(uint32_t*) (&state[4]) ^= rk[1];
    *(uint32_t*) (&state[8]) ^= rk[2];
    *(uint32_t*) (&state[12]) ^= rk[3];
}

#define f2(x) ((x << 1) ^ (((x >> 7) & 1) * 0x11b))
#define f4(x) ((x << 2) ^ (((x >> 6) & 1) * 0x11b) ^ (((x >> 6) & 2) * 0x11b))
#define f8(x) ((x << 3) ^ (((x >> 5) & 1) * 0x11b) ^ (((x >> 5) & 2) * 0x11b) ^ (((x >> 5) & 4) * 0x11b))

#define f3(x) (f2(x) ^ x)
#define f9(x) (f8(x) ^ x)
#define fb(x) (f8(x) ^ f2(x) ^ x)
#define fd(x) (f8(x) ^ f4(x) ^ x)
#define fe(x) (f8(x) ^ f4(x) ^ f2(x))

uint32_t Aes::invMixColumns(uint32_t x) const
{
    uint8_t a = SBOX[Utils::word2byte(x, 0)];
    uint8_t b = SBOX[Utils::word2byte(x, 1)];
    uint8_t c = SBOX[Utils::word2byte(x, 2)];
    uint8_t d = SBOX[Utils::word2byte(x, 3)];
    return Td[0 * 256 + d] ^ Td[1 * 256 + c] ^ Td[2 * 256 + b] ^ Td[3 * 256 + a];
}

void Aes::fillEncTable() const
{
    for (int i = 0; i < 256; i++)
    {
        uint8_t x = SBOX[i];
        uint32_t y = Utils::byte2word(f2(x), f3(x), x, x);
        for (int j = 0; j < 4; j++)
        {
            Te[j * 256 + i] = y;
            y = Utils::rot_right(y, 8);
        }
    }
}

void Aes::fillDecTable() const
{
    for (int i = 0; i < 256; i++)
    {
        uint8_t x = ISBOX[i];
        uint32_t y = Utils::byte2word(fe(x), fb(x), fd(x), f9(x));
        for (int j = 0; j < 4; j++)
        {
            Td[j * 256 + i] = y;
            y = Utils::rot_right(y, 8);
        }
    }
}

void Aes::keyExpansion(const ByteArray& key)
{
    assert(key.length() >= Nk * 4);

    int len = Nb * (Nr + 1);
    m_w = new uint32_t[len];
    m_dw = new uint32_t[len];
    int i = 0;
    for (; i < Nk; i++)
        m_w[i] = Utils::byte2word(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
    for (; i < len; i++)
    {
        uint32_t temp = m_w[i - 1];
        if (i % Nk == 0)
            temp = Utils::sub_word(Utils::rot_left(temp, 8), SBOX) ^ RCON[i / Nk - 1];
        else if (Nk > 6 && i % Nk == 4)
            temp = Utils::sub_word(temp, SBOX);
        m_w[i] = m_w[i - Nk] ^ temp;
    }
    for (int i = 0; i < len; i++)
        m_w[i] = Utils::reverse_word(m_w[i]);
    for (int i = 0; i <= Nr; i++)
    {
        memcpy(&m_dw[i * 4], &m_w[(Nr - i) * 4], sizeof(uint32_t) * 4);
        if (i && i < Nr)
        {
            for (int j = 0; j < 4; j++)
                m_dw[i * 4 + j] = invMixColumns(m_dw[i * 4 + j]);
        }
    }
}

void Aes::printState(const uint8_t* state) const
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < Nb; j++)
            printf("  0x%02x", state[j * 4 + i]);
        printf("\n");
    }
    printf("\n");
}
