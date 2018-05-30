#include <assert.h>

#include "aes.h"

const uint8_t Aes::SBOX[16][16] = {
    {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
    {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
    {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
    {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
    {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
    {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
    {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
    {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
    {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
    {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
    {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
    {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
    {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
    {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
    {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
    {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16},
};

const uint8_t Aes::ISBOX[16][16] = {
    {0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB},
    {0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB},
    {0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E},
    {0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25},
    {0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92},
    {0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84},
    {0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06},
    {0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B},
    {0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73},
    {0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E},
    {0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B},
    {0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4},
    {0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F},
    {0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF},
    {0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61},
    {0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D},
};

const uint32_t Aes::RCON[10] = {
    0x01000000,
    0x02000000,
    0x04000000,
    0x08000000,
    0x10000000,
    0x20000000,
    0x40000000,
    0x80000000,
    0x1b000000,
    0x36000000,
};

Aes::Aes(AESKeyLength keyLength, const ByteArray& key, AESMode mode)
    : m_mode(mode), m_w(nullptr), m_iv(16)
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
    keyExpansion(key.length() >= Nk * 4 ? key : key.padding(Nk * 4));
}

Aes::~Aes()
{
    if (m_w)
        delete[] m_w;
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
            for (int j = 0; j < m_block_bytes; j++)
                state[i + j] ^= last[j];
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
            for (int j = 0; j < m_block_bytes; j++)
                state[i + j] ^= last[j];
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
        return ByteArray("ERROR");
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
    addRoundKey(state, &m_w[0]);
    for (int i = 1; i <= Nr; i++)
    {
        subBytes(state);
        shiftRows(state);
        if (i < Nr) mixColumns(state);
        addRoundKey(state, &m_w[i * 4]);
    }
}

void Aes::decryptOneBlock(uint8_t* state) const
{
    addRoundKey(state, &m_w[Nr * 4]);
    for (int i = Nr - 1; i >= 0; i--)
    {
        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(state, &m_w[i * 4]);
        if (i > 0) invMixColumns(state);
    }
}

void Aes::addRoundKey(uint8_t* state, const uint32_t* roundKey) const
{
    for (int i = 0; i < Nb; i++)
    {
        for (int j = 0; j < 4; j++)
            state[i * 4 + j] ^= Utils::word2byte(roundKey[i], 3 - j);
    }
}

void Aes::subBytes(uint8_t* state) const
{
    for (int i = 0; i < m_block_bytes; i++)
        state[i] = Utils::sub_byte(state[i], SBOX);
}

void Aes::invSubBytes(uint8_t* state) const
{
    for (int i = 0; i < m_block_bytes; i++)
        state[i] = Utils::sub_byte(state[i], ISBOX);
}

void Aes::shiftRows(uint8_t* state) const
{
    assert(m_block_bytes == 16);
    uint8_t tmp = state[1];
    state[1] = state[5], state[5] = state[9], state[9] = state[13], state[13] = tmp;
    tmp = state[3];
    state[3] = state[15], state[15] = state[11], state[11] = state[7], state[7] = tmp;
    tmp = state[2], state[2] = state[10], state[10] = tmp;
    tmp = state[6], state[6] = state[14], state[14] = tmp;
}

void Aes::invShiftRows(uint8_t* state) const
{
    assert(m_block_bytes == 16);
    uint8_t tmp = state[1];
    state[1] = state[13], state[13] = state[9], state[9] = state[5], state[5] = tmp;
    tmp = state[3];
    state[3] = state[7], state[7] = state[11], state[11] = state[15], state[15] = tmp;
    tmp = state[2], state[2] = state[10], state[10] = tmp;
    tmp = state[6], state[6] = state[14], state[14] = tmp;
}

void Aes::mixColumns(uint8_t* state) const
{
    assert(Nb == 4);
    for (int i = 0; i < m_block_bytes; i += 4)
    {
        uint8_t a = state[i];
        uint8_t b = state[i + 1];
        uint8_t c = state[i + 2];
        uint8_t d = state[i + 3];
        uint8_t e = a ^ b ^ c ^ d;

        state[i] ^= e ^ Utils::xtime(a ^ b);
        state[i + 1] ^= e ^ Utils::xtime(b ^ c);
        state[i + 2] ^= e ^ Utils::xtime(c ^ d);
        state[i + 3] ^= e ^ Utils::xtime(d ^ a);
    }
}

void Aes::invMixColumns(uint8_t* state) const
{
    assert(Nb == 4);
    for (int i = 0; i < m_block_bytes; i += 4)
    {
        uint8_t a = state[i];
        uint8_t b = state[i + 1];
        uint8_t c = state[i + 2];
        uint8_t d = state[i + 3];
        uint8_t e = a ^ b ^ c ^ d, e2 = Utils::xtime(e);
        uint8_t ac = Utils::xtime(e2 ^ a ^ c), bd = Utils::xtime(e2 ^ b ^ d);

        state[i] ^= e ^ Utils::xtime(a ^ b ^ ac);
        state[i + 1] ^= e ^ Utils::xtime(b ^ c ^ bd);
        state[i + 2] ^= e ^ Utils::xtime(c ^ d ^ ac);
        state[i + 3] ^= e ^ Utils::xtime(d ^ a ^ bd);
    }
}

void Aes::keyExpansion(const ByteArray& key)
{
    assert(key.length() >= Nk * 4);

    int len = Nb * (Nr + 1);
    m_w = new uint32_t[len];
    int i = 0;
    for (; i < Nk; i++)
        m_w[i] = Utils::byte2word(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
    for (; i < len; i++)
    {
        uint32_t temp = m_w[i - 1];
        if (i % Nk == 0)
            temp = Utils::sub_word(Utils::rot_word(temp), SBOX) ^ RCON[i / Nk - 1];
        else if (Nk > 6 && i % Nk == 4)
            temp = Utils::sub_word(temp, SBOX);
        m_w[i] = m_w[i - Nk] ^ temp;
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
