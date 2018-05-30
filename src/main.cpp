#include "aes.h"

const char key[] = "abcdefghijklmnop";

int main()
{
    Aes aes(Aes::AES_128, ByteArray(key));
    aes.setInitializationVector(ByteArray("1234567890123456"));

    ByteArray text = ByteArray::fromFile("../aes.cpp");
    ByteArray cipher = aes.encrypt(text);
    cipher.saveToFile("aes.cpp.enc");
    ByteArray text2 = aes.decrypt(ByteArray::fromFile("aes.cpp.enc"));
    text2.saveToFile("aes.cpp.dec");

    printf("%s\n", cipher.toHexString().c_str());
    printf("%s\n", text2.toPlainText().c_str());

    return 0;
}
