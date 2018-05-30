#include "aes.h"

const char input[] = "abcdefghijklmnop";
const char key_128[] = "abcdefghijklmnop";
const char key_192[] = "abcdefghijklmnopabcdefgh";
const char key_256[] = "abcdefghijklmnopabcdefghijklmnop";

int main()
{
    Aes aes128(Aes::AES_128, ByteArray(key_128));
    Aes aes192(Aes::AES_192, ByteArray(key_192));
    Aes aes256(Aes::AES_256, ByteArray(key_256));

    ByteArray cipher128 = aes128.encrypt(ByteArray(input));
    ByteArray cipher192 = aes192.encrypt(ByteArray(input));
    ByteArray cipher256 = aes256.encrypt(ByteArray(input));

    ByteArray dec128 = aes128.decrypt(ByteArray::fromHexString(cipher128.toHexString().c_str()));
    ByteArray dec192 = aes192.decrypt(ByteArray::fromHexString(cipher192.toHexString().c_str()));
    ByteArray dec256 = aes256.decrypt(ByteArray::fromHexString(cipher256.toHexString().c_str()));

    printf("input: %s\n", ByteArray(input).toPlainText().c_str());

    printf("cipher(128): %s\n", cipher128.toHexString().c_str());
    // a91329af99a78d02aec17c507757aaefcc8ca6fe020a55ba40fef9583a7b1311
    printf("cipher(192): %s\n", cipher192.toHexString().c_str());
    // 3126ffb3ec87aab9e46b846d20b3c47507c558f07ff992e97057950aa1514bca
    printf("cipher(256): %s\n", cipher256.toHexString().c_str());
    // 69937a27dc9d720a960a661524c803a2398fe0a2e0369f6b6714679941e06c8b

    printf("decrypted(128): %s\n", dec128.toPlainText().c_str());
    printf("decrypted(192): %s\n", dec192.toPlainText().c_str());
    printf("decrypted(256): %s\n", dec256.toPlainText().c_str());

    return 0;
}

