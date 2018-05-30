#include "aes.h"

const char text[] = "0123456789012345";
const char key[] = "FEDCBA9876543210";

int main()
{
    Aes aes(Aes::AES_128, Aes::AES_CBC_Mode, ByteArray(key));

    ByteArray cipher = aes.encode(ByteArray(text));
    ByteArray text2 = aes.decode(cipher);

    printf("%s\n", cipher.toHexString().c_str());
    printf("%s\n", text2.toPlainText().c_str());

    return 0;
}
