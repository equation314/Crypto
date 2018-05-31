# Projects of Modern Cryptography

Implementation of AES & keccak algorithms.

## Build

```
mkdir build & cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

## AES

### Run

```
Usage: ./mainaes [options] <INPUT_FILE> -o <OUTPUT_FILE>
Options:
  -k <key>                  Key for encryption/decryption.
  -e                        Encrypt <INPUT_FILE>.
  -d                        Decrypt <INPUT_FILE>.
  -l [128|192|256]          Key length used for AES (default is 128).
  -m [CBC|ECB]              Block cipher mode of operation (default is CBC).
  --help                    Display this information.
```

### Test

1. Create a large file (about 100 MiB) named `input100m.bin`, and place it in the `build` directory.

2. Run the test program:

    ```
    ./testaes
    ```
3. The output will look like the following:

    ```
    input: abcdefghijklmnop
    cipher(128): a91329af99a78d02aec17c507757aaefcc8ca6fe020a55ba40fef9583a7b1311
    cipher(192): 3126ffb3ec87aab9e46b846d20b3c47507c558f07ff992e97057950aa1514bca
    cipher(256): 69937a27dc9d720a960a661524c803a2398fe0a2e0369f6b6714679941e06c8b
    decrypted(128): abcdefghijklmnop
    decrypted(192): abcdefghijklmnop
    decrypted(256): abcdefghijklmnop
    testcase #0 passed. (0.000s)
    Time: 9.461 s
    Speed: 211.399 MB/s
    testcase #1 passed. (9.687s)
    ```
