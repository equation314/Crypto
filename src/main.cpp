#include "aes.h"

const char text[] = "abcdefghijklmnop";
const char key[] = "abcdefghijklmnop";

int main()
{
    Aes aes(Aes::AES_128, Aes::AES_CBC_Mode, ByteArray(key));

    ByteArray cipher = aes.encrypt(ByteArray(text));
    ByteArray text2 = aes.decrypt(cipher);

    printf("%s\n", cipher.toHexString().c_str());
    printf("%s\n", text2.toPlainText().c_str());

    return 0;
}
