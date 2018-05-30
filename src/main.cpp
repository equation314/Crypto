#include "aes.h"

const char text[] = "0123456789012345";
const uint8_t key[] = {0x3C, 0xA1, 0x0B, 0x21, 0x57, 0xF0, 0x19, 0x16, 0x90, 0x2E, 0x13, 0x80, 0xAC, 0xC1, 0x07, 0xBD};

int main()
{
    Aes aes(Aes::AES_128, Aes::AES_CBC_Mode, ByteArray(key, 16));

    ByteArray cipher = aes.encode(ByteArray(text));
    ByteArray text2 = aes.decode(cipher);

    printf("%s\n", cipher.toHexString().c_str());
    printf("%s\n", text2.toPlainText().c_str());

    return 0;
}
