# Projects of Modern Cryptography

Implementation of AES and SHA3(Keccak) algorithms.

## Build

```
mkdir build & cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

## Run

### AES

```
Usage: ./aes [options] <INPUT_FILE> -o <OUTPUT_FILE>
Options:
  -k <key>                  Key for encryption/decryption.
  -e                        Encrypt <INPUT_FILE>.
  -d                        Decrypt <INPUT_FILE>.
  -l [128|192|256]          Key length used for AES (default is 128).
  -m [CBC|ECB]              Block cipher mode of operation (default is CBC).
  --help                    Display this information.
```

### SHA3

## Test

1. Create a large file (about 100 MiB) named `input100m.bin`, and place it in the `build` directory.

2. Run the test program:

    ```
    ./crypto_test
    ```

3. The output will look like the following:

    ```
    ==================== Testcase #0 ====================
    message:
    SHA3-224: 6b4e03423667dbb73b6e15454f0eb1abd4597f9a1b078e3f5b5a6bc7
    SHA3_256: a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a
    SHA3_384: 0c63a75b845e4f7d01107d852e4c2485c51a50aaaa94fc61995e71bbee983a2ac3713831264adb47fb6bd1e058d5f004
    SHA3_512: a69f73cca23a9ac5c8b567dc185a756e97c982164fe25859e0d1dcc1475c80a615b2123af1f5f94c11e3e9402c3ac558f500199d95b6d3e301758586281dcd26
    message: abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq
    SHA3-224: 8a24108b154ada21c9fd5574494479ba5c7e7ab76ef264ead0fcce33
    SHA3_256: 41c0dba2a9d6240849100376a8235e2c82e1b9998a999e21db32dd97496d3376
    SHA3_384: 991c665755eb3a4b6bbdfb75c78a492e8c56a22c5c4d7e429bfdbc32b9d4ad5aa04a1f076e62fea19eef51acd0657c22
    SHA3_512: 04a371e84ecfb5b8b77cb48610fca8182dd457ce6f326a0fd3d7ec2f1e91636dee691fbe0c985302ba1b0d8dc78c086346b533b49c030d99a27daf1139d6e75e
    ============ Testcase #0 passed (0.000s) ============

    ==================== Testcase #1 ====================
    Time: 7.376 s
    Speed: 135.571 MiB/s
    ============ Testcase #1 passed (7.467s) ============

    ==================== Testcase #2 ====================
    input: abcdefghijklmnop
    cipher(128): a91329af99a78d02aec17c507757aaefcc8ca6fe020a55ba40fef9583a7b1311
    cipher(192): 3126ffb3ec87aab9e46b846d20b3c47507c558f07ff992e97057950aa1514bca
    cipher(256): 69937a27dc9d720a960a661524c803a2398fe0a2e0369f6b6714679941e06c8b
    decrypted(128): abcdefghijklmnop
    decrypted(192): abcdefghijklmnop
    decrypted(256): abcdefghijklmnop
    ============ Testcase #2 passed (0.000s) ============

    ==================== Testcase #3 ====================
    Time: 9.033 s
    Speed: 221.403 MiB/s
    ============ Testcase #3 passed (9.157s) ============

    ```
