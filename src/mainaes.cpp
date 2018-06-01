#include "aes.h"

void showHelp(const char* argv0)
{
    printf("Usage: %s [options] <INPUT_FILE> -o <OUTPUT_FILE>\n"
           "Options:\n"
           "  -k <key>                  Key for encryption/decryption.\n"
           "  -e                        Encrypt <INPUT_FILE>.\n"
           "  -d                        Decrypt <INPUT_FILE>.\n"
           "  -l [128|192|256]          Key length used for AES (default is 128).\n"
           "  -m [CBC|ECB]              Block cipher mode of operation (default is CBC).\n"
           "  --help                    Display this information.\n",
           argv0);
    exit(0);
}

int main(int argc, char* argv[])
{
    Aes::AESMode mode = Aes::AES_CBC_Mode;
    Aes::AESKeyLength keyLength = Aes::AES_128;
    bool isEncryption = true;
    std::string key = "";
    std::string inFile = "", outFile = "";

    if (argc > 1)
        inFile = argv[1];
    for (int i = 2; i < argc; i++)
    {
        if (!strcmp(argv[i], "-e"))
            isEncryption = true;
        else if (!strcmp(argv[i], "-d"))
            isEncryption = false;
        else if (!strcmp(argv[i], "-k"))
        {
            if (i + 1 < argc)
            {
                key = argv[i + 1];
                i++;
            }
            else
                showHelp(argv[0]);
        }
        else if (!strcmp(argv[i], "-l"))
        {
            if (i + 1 < argc)
            {
                if (!strcmp(argv[i + 1], "128"))
                    keyLength = Aes::AES_128;
                else if (!strcmp(argv[i + 1], "192"))
                    keyLength = Aes::AES_192;
                else if (!strcmp(argv[i + 1], "256"))
                    keyLength = Aes::AES_256;
                else
                    showHelp(argv[0]);
                i++;
            }
            else
                showHelp(argv[0]);
        }
        else if (!strcmp(argv[i], "-m"))
        {
            if (i + 1 < argc)
            {
                if (!strcmp(argv[i + 1], "CBC"))
                    mode = Aes::AES_CBC_Mode;
                else if (!strcmp(argv[i + 1], "ECB"))
                    mode = Aes::AES_ECB_Mode;
                else
                    showHelp(argv[0]);
                i++;
            }
            else
                showHelp(argv[0]);
        }
        else if (!strcmp(argv[i], "-o"))
        {
            if (i + 1 < argc)
            {
                outFile = argv[i + 1];
                i++;
            }
            else
                showHelp(argv[0]);
        }
        else if (!strcmp(argv[i], "--help"))
            showHelp(argv[0]);
    }
    if (!key.length() || !inFile.length())
        showHelp(argv[0]);

    if (!outFile.length())
        outFile = inFile + (isEncryption ? ".enc" : ".dec");

    Aes aes(keyLength, ByteArray(key.c_str()), mode);
    aes.setInitializationVector(ByteArray("1234567890123456"));

    ByteArray input = ByteArray::fromFile(inFile.c_str());
    ByteArray output;
    if (isEncryption)
        output = aes.encrypt(input);
    else
        output = aes.decrypt(input);

    if (output.isError())
        printf("ERROR!\n");
    else
    {
        output.saveToFile(outFile.c_str());
        printf("%s %s completed.\nThe output file is \"%s\"\n", aes.algorithmName().c_str(),
               isEncryption ? "encryption" : "decryption", outFile.c_str());
    }

    return 0;
}
