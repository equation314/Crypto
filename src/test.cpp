#include <ctime>
#include <string>
#include <assert.h>

#include "aes.h"
#include "sha3.h"

void test_aes_1()
{
    const char input[] = "abcdefghijklmnop";
    const char key_128[] = "abcdefghijklmnop";
    const char key_192[] = "abcdefghijklmnopabcdefgh";
    const char key_256[] = "abcdefghijklmnopabcdefghijklmnop";

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
    assert(!strcmp(cipher128.toHexString().c_str(),
                   "a91329af99a78d02aec17c507757aaefcc8ca6fe020a55ba40fef9583a7b1311"));

    printf("cipher(192): %s\n", cipher192.toHexString().c_str());
    assert(!strcmp(cipher192.toHexString().c_str(),
                   "3126ffb3ec87aab9e46b846d20b3c47507c558f07ff992e97057950aa1514bca"));

    printf("cipher(256): %s\n", cipher256.toHexString().c_str());
    assert(!strcmp(cipher256.toHexString().c_str(),
                   "69937a27dc9d720a960a661524c803a2398fe0a2e0369f6b6714679941e06c8b"));

    printf("decrypted(128): %s\n", dec128.toPlainText().c_str());
    assert(!strcmp(dec128.toPlainText().c_str(), "abcdefghijklmnop"));

    printf("decrypted(192): %s\n", dec192.toPlainText().c_str());
    assert(!strcmp(dec192.toPlainText().c_str(), "abcdefghijklmnop"));

    printf("decrypted(256): %s\n", dec256.toPlainText().c_str());
    assert(!strcmp(dec256.toPlainText().c_str(), "abcdefghijklmnop"));
}

void test_aes_2()
{
    const int round = 10;
    const std::string fileName = "input100m.bin";
    const char key_128[] = "3141592653589793";

    Aes aes128(Aes::AES_128, ByteArray(key_128));
    ByteArray input = ByteArray::fromFile(fileName.c_str());
    ByteArray output = input;

    long long size = 0;
    int begin = clock();
    for (int i = 0; i < round; i++)
    {
        size += output.length();
        output = aes128.encrypt(output);
    }

    for (int i = 0; i < round; i++)
    {
        size += output.length();
        output = aes128.decrypt(output);
    }

    int end = clock();
    double timeUsed = (1.0 * (end - begin) / CLOCKS_PER_SEC);
    printf("Time: %.3lf s\n", timeUsed);
    printf("Speed: %.3lf MiB/s\n", 1.0 * size / (1 << 20) / timeUsed);

    assert(input == output);
}

void test_sha3_1()
{
    Sha3 sha3_224(Sha3::SHA3_224);
    Sha3 sha3_256(Sha3::SHA3_256);
    Sha3 sha3_384(Sha3::SHA3_384);
    Sha3 sha3_512(Sha3::SHA3_512);
    ByteArray message, d224, d256, d384, d512;

    message = ByteArray("");
    printf("message: %s\n", message.toPlainText().c_str());

    d224 = sha3_224.hash(message);
    d256 = sha3_256.hash(message);
    d384 = sha3_384.hash(message);
    d512 = sha3_512.hash(message);

    printf("SHA3-224: %s\n", d224.toHexString().c_str());
    assert(!strcmp(d224.toHexString().c_str(),
                   "6b4e03423667dbb73b6e15454f0eb1abd4597f9a1b078e3f5b5a6bc7"));

    printf("SHA3_256: %s\n", d256.toHexString().c_str());
    assert(!strcmp(d256.toHexString().c_str(),
                   "a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a"));

    printf("SHA3_384: %s\n", d384.toHexString().c_str());
    assert(!strcmp(d384.toHexString().c_str(),
                   "0c63a75b845e4f7d01107d852e4c2485c51a50aaaa94fc61995e71bbee983a2ac3713831264adb47fb6bd1e058d5f004"));

    printf("SHA3_512: %s\n", d512.toHexString().c_str());
    assert(!strcmp(d512.toHexString().c_str(),
                   "a69f73cca23a9ac5c8b567dc185a756e97c982164fe25859e0d1dcc1475c80a615b2123af1f5f94c11e3e9402c3ac558f500199d95b6d3e301758586281dcd26"));

    message = ByteArray("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
    printf("message: %s\n", message.toPlainText().c_str());

    d224 = sha3_224.hash(message);
    d256 = sha3_256.hash(message);
    d384 = sha3_384.hash(message);
    d512 = sha3_512.hash(message);

    printf("SHA3-224: %s\n", d224.toHexString().c_str());
    assert(!strcmp(d224.toHexString().c_str(),
                   "8a24108b154ada21c9fd5574494479ba5c7e7ab76ef264ead0fcce33"));

    printf("SHA3_256: %s\n", d256.toHexString().c_str());
    assert(!strcmp(d256.toHexString().c_str(),
                   "41c0dba2a9d6240849100376a8235e2c82e1b9998a999e21db32dd97496d3376"));

    printf("SHA3_384: %s\n", d384.toHexString().c_str());
    assert(!strcmp(d384.toHexString().c_str(),
                   "991c665755eb3a4b6bbdfb75c78a492e8c56a22c5c4d7e429bfdbc32b9d4ad5aa04a1f076e62fea19eef51acd0657c22"));

    printf("SHA3_512: %s\n", d512.toHexString().c_str());
    assert(!strcmp(d512.toHexString().c_str(),
                   "04a371e84ecfb5b8b77cb48610fca8182dd457ce6f326a0fd3d7ec2f1e91636dee691fbe0c985302ba1b0d8dc78c086346b533b49c030d99a27daf1139d6e75e"));
}

void test_sha3_2()
{
    const int round = 20;
    const std::string fileName = "input100m.bin";

    Sha3 sha3(Sha3::SHA3_256);
    ByteArray input = ByteArray::fromFile(fileName.c_str());
    ByteArray output;

    long long size = 0;
    int begin = clock();
    for (int i = 0; i < round; i++)
    {
        size += input.length();
        output = sha3.hash(input);
    }

    int end = clock();
    double timeUsed = (1.0 * (end - begin) / CLOCKS_PER_SEC);
    printf("Time: %.3lf s\n", timeUsed);
    printf("Speed: %.3lf MiB/s\n", 1.0 * size / (1 << 20) / timeUsed);
}

int main()
{
    void (*testcases[])() = {
        test_sha3_1,
        test_sha3_2,
        test_aes_1,
        test_aes_2,
    };

    for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
    {
        printf("==================== Testcase #%d ====================\n", i);
        int begin = clock();
        testcases[i]();
        int end = clock();
        printf("============ Testcase #%d passed (%.3lfs) ============\n\n",
               i, 1.0 * (end - begin) / CLOCKS_PER_SEC);
    }
    return 0;
}
