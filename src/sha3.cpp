#include "sha3.h"
#include "sha3const.h"

Sha3::Sha3()
{
}

Sha3::~Sha3()
{
}

void Sha3::keccak_f(uint64_t* state)
{
#define A(x, y) state[(y) *5 + (x)]

    uint64_t Aba = A(0, 0), Abe = A(1, 0), Abi = A(2, 0), Abo = A(3, 0), Abu = A(4, 0);
    uint64_t Aga = A(0, 1), Age = A(1, 1), Agi = A(2, 1), Ago = A(3, 1), Agu = A(4, 1);
    uint64_t Aka = A(0, 2), Ake = A(1, 2), Aki = A(2, 2), Ako = A(3, 2), Aku = A(4, 2);
    uint64_t Ama = A(0, 3), Ame = A(1, 3), Ami = A(2, 3), Amo = A(3, 3), Amu = A(4, 3);
    uint64_t Asa = A(0, 4), Ase = A(1, 4), Asi = A(2, 4), Aso = A(3, 4), Asu = A(4, 4);
    uint64_t BCa, BCe, BCi, BCo, BCu;
    uint64_t Da, De, Di, Do, Du;
    uint64_t Eba, Ebe, Ebi, Ebo, Ebu;
    uint64_t Ega, Ege, Egi, Ego, Egu;
    uint64_t Eka, Eke, Eki, Eko, Eku;
    uint64_t Ema, Eme, Emi, Emo, Emu;
    uint64_t Esa, Ese, Esi, Eso, Esu;

    for (unsigned int round = 0; round < 24; round += 2)
    {
        //    prepareTheta
        BCa = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
        BCe = Abe ^ Age ^ Ake ^ Ame ^ Ase;
        BCi = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
        BCo = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
        BCu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;

        //thetaRhoPiChiIotaPrepareTheta(round  , A, E)
        Da = BCu ^ Utils::rot_left(BCe, 1);
        De = BCa ^ Utils::rot_left(BCi, 1);
        Di = BCe ^ Utils::rot_left(BCo, 1);
        Do = BCi ^ Utils::rot_left(BCu, 1);
        Du = BCo ^ Utils::rot_left(BCa, 1);

        Aba ^= Da;
        BCa = Aba;
        Age ^= De;
        BCe = Utils::rot_left(Age, 44);
        Aki ^= Di;
        BCi = Utils::rot_left(Aki, 43);
        Amo ^= Do;
        BCo = Utils::rot_left(Amo, 21);
        Asu ^= Du;
        BCu = Utils::rot_left(Asu, 14);
        Eba = BCa ^ ((~BCe) & BCi);
        Eba ^= ROUND_CONST[round];
        Ebe = BCe ^ ((~BCi) & BCo);
        Ebi = BCi ^ ((~BCo) & BCu);
        Ebo = BCo ^ ((~BCu) & BCa);
        Ebu = BCu ^ ((~BCa) & BCe);

        Abo ^= Do;
        BCa = Utils::rot_left(Abo, 28);
        Agu ^= Du;
        BCe = Utils::rot_left(Agu, 20);
        Aka ^= Da;
        BCi = Utils::rot_left(Aka, 3);
        Ame ^= De;
        BCo = Utils::rot_left(Ame, 45);
        Asi ^= Di;
        BCu = Utils::rot_left(Asi, 61);
        Ega = BCa ^ ((~BCe) & BCi);
        Ege = BCe ^ ((~BCi) & BCo);
        Egi = BCi ^ ((~BCo) & BCu);
        Ego = BCo ^ ((~BCu) & BCa);
        Egu = BCu ^ ((~BCa) & BCe);

        Abe ^= De;
        BCa = Utils::rot_left(Abe, 1);
        Agi ^= Di;
        BCe = Utils::rot_left(Agi, 6);
        Ako ^= Do;
        BCi = Utils::rot_left(Ako, 25);
        Amu ^= Du;
        BCo = Utils::rot_left(Amu, 8);
        Asa ^= Da;
        BCu = Utils::rot_left(Asa, 18);
        Eka = BCa ^ ((~BCe) & BCi);
        Eke = BCe ^ ((~BCi) & BCo);
        Eki = BCi ^ ((~BCo) & BCu);
        Eko = BCo ^ ((~BCu) & BCa);
        Eku = BCu ^ ((~BCa) & BCe);

        Abu ^= Du;
        BCa = Utils::rot_left(Abu, 27);
        Aga ^= Da;
        BCe = Utils::rot_left(Aga, 36);
        Ake ^= De;
        BCi = Utils::rot_left(Ake, 10);
        Ami ^= Di;
        BCo = Utils::rot_left(Ami, 15);
        Aso ^= Do;
        BCu = Utils::rot_left(Aso, 56);
        Ema = BCa ^ ((~BCe) & BCi);
        Eme = BCe ^ ((~BCi) & BCo);
        Emi = BCi ^ ((~BCo) & BCu);
        Emo = BCo ^ ((~BCu) & BCa);
        Emu = BCu ^ ((~BCa) & BCe);

        Abi ^= Di;
        BCa = Utils::rot_left(Abi, 62);
        Ago ^= Do;
        BCe = Utils::rot_left(Ago, 55);
        Aku ^= Du;
        BCi = Utils::rot_left(Aku, 39);
        Ama ^= Da;
        BCo = Utils::rot_left(Ama, 41);
        Ase ^= De;
        BCu = Utils::rot_left(Ase, 2);
        Esa = BCa ^ ((~BCe) & BCi);
        Ese = BCe ^ ((~BCi) & BCo);
        Esi = BCi ^ ((~BCo) & BCu);
        Eso = BCo ^ ((~BCu) & BCa);
        Esu = BCu ^ ((~BCa) & BCe);

        //    prepareTheta
        BCa = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
        BCe = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
        BCi = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
        BCo = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
        BCu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;

        //thetaRhoPiChiIotaPrepareTheta(round+1, E, A)
        Da = BCu ^ Utils::rot_left(BCe, 1);
        De = BCa ^ Utils::rot_left(BCi, 1);
        Di = BCe ^ Utils::rot_left(BCo, 1);
        Do = BCi ^ Utils::rot_left(BCu, 1);
        Du = BCo ^ Utils::rot_left(BCa, 1);

        Eba ^= Da;
        BCa = Eba;
        Ege ^= De;
        BCe = Utils::rot_left(Ege, 44);
        Eki ^= Di;
        BCi = Utils::rot_left(Eki, 43);
        Emo ^= Do;
        BCo = Utils::rot_left(Emo, 21);
        Esu ^= Du;
        BCu = Utils::rot_left(Esu, 14);
        Aba = BCa ^ ((~BCe) & BCi);
        Aba ^= ROUND_CONST[round + 1];
        Abe = BCe ^ ((~BCi) & BCo);
        Abi = BCi ^ ((~BCo) & BCu);
        Abo = BCo ^ ((~BCu) & BCa);
        Abu = BCu ^ ((~BCa) & BCe);

        Ebo ^= Do;
        BCa = Utils::rot_left(Ebo, 28);
        Egu ^= Du;
        BCe = Utils::rot_left(Egu, 20);
        Eka ^= Da;
        BCi = Utils::rot_left(Eka, 3);
        Eme ^= De;
        BCo = Utils::rot_left(Eme, 45);
        Esi ^= Di;
        BCu = Utils::rot_left(Esi, 61);
        Aga = BCa ^ ((~BCe) & BCi);
        Age = BCe ^ ((~BCi) & BCo);
        Agi = BCi ^ ((~BCo) & BCu);
        Ago = BCo ^ ((~BCu) & BCa);
        Agu = BCu ^ ((~BCa) & BCe);

        Ebe ^= De;
        BCa = Utils::rot_left(Ebe, 1);
        Egi ^= Di;
        BCe = Utils::rot_left(Egi, 6);
        Eko ^= Do;
        BCi = Utils::rot_left(Eko, 25);
        Emu ^= Du;
        BCo = Utils::rot_left(Emu, 8);
        Esa ^= Da;
        BCu = Utils::rot_left(Esa, 18);
        Aka = BCa ^ ((~BCe) & BCi);
        Ake = BCe ^ ((~BCi) & BCo);
        Aki = BCi ^ ((~BCo) & BCu);
        Ako = BCo ^ ((~BCu) & BCa);
        Aku = BCu ^ ((~BCa) & BCe);

        Ebu ^= Du;
        BCa = Utils::rot_left(Ebu, 27);
        Ega ^= Da;
        BCe = Utils::rot_left(Ega, 36);
        Eke ^= De;
        BCi = Utils::rot_left(Eke, 10);
        Emi ^= Di;
        BCo = Utils::rot_left(Emi, 15);
        Eso ^= Do;
        BCu = Utils::rot_left(Eso, 56);
        Ama = BCa ^ ((~BCe) & BCi);
        Ame = BCe ^ ((~BCi) & BCo);
        Ami = BCi ^ ((~BCo) & BCu);
        Amo = BCo ^ ((~BCu) & BCa);
        Amu = BCu ^ ((~BCa) & BCe);

        Ebi ^= Di;
        BCa = Utils::rot_left(Ebi, 62);
        Ego ^= Do;
        BCe = Utils::rot_left(Ego, 55);
        Eku ^= Du;
        BCi = Utils::rot_left(Eku, 39);
        Ema ^= Da;
        BCo = Utils::rot_left(Ema, 41);
        Ese ^= De;
        BCu = Utils::rot_left(Ese, 2);
        Asa = BCa ^ ((~BCe) & BCi);
        Ase = BCe ^ ((~BCi) & BCo);
        Asi = BCi ^ ((~BCo) & BCu);
        Aso = BCo ^ ((~BCu) & BCa);
        Asu = BCu ^ ((~BCa) & BCe);
    }

    A(0, 0) = Aba, A(1, 0) = Abe, A(2, 0) = Abi, A(3, 0) = Abo, A(4, 0) = Abu;
    A(0, 1) = Aga, A(1, 1) = Age, A(2, 1) = Agi, A(3, 1) = Ago, A(4, 1) = Agu;
    A(0, 2) = Aka, A(1, 2) = Ake, A(2, 2) = Aki, A(3, 2) = Ako, A(4, 2) = Aku;
    A(0, 3) = Ama, A(1, 3) = Ame, A(2, 3) = Ami, A(3, 3) = Amo, A(4, 3) = Amu;
    A(0, 4) = Asa, A(1, 4) = Ase, A(2, 4) = Asi, A(3, 4) = Aso, A(4, 4) = Asu;

#undef A
}

ByteArray paddingBuffer(const ByteArray& buffer, uint8_t align)
{
    int padding = align - buffer.length() % align;
    ByteArray padBuffer(buffer.length() + padding);
    ByteArray::copy(padBuffer, 0, buffer, 0, buffer.length());

    padBuffer[buffer.length()] = 0x01;
    padBuffer[padBuffer.length() - 1] ^= 0x80;
    return padBuffer;
}

ByteArray Sha3::hash(const ByteArray& buf)
{
    const int output_size = 512;
    const int c = output_size * 2;
    const int r = (1600 - c) / 8;
    const int r_len = r / sizeof(uint64_t);

    ByteArray buffer = paddingBuffer(buf, r);
    uint64_t* ptr = (uint64_t*) &buffer[0];
    memset(m_state, 0, sizeof(m_state));

    for (int i = 0; i < buffer.length() / r; i++)
    {
        for (int j = 0; j < r_len; j++)
            m_state[j] ^= ptr[i * r_len + j];
        keccak_f(m_state);
    }

    ByteArray output(output_size / sizeof(uint64_t));
    ptr = (uint64_t*) &output[0];

    for (int t = 0; t < 8;)
        for (int j = 0; j < r_len && t < 8; j++)
            ptr[t++] ^= m_state[j];

    return output;
}
