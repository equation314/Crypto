#include "sha3.h"

void showHelp(const char* argv0)
{
    printf("Usage: %s [options]>\n"
           "Options:\n"
           "  -m <MESSAGE>              Print the SHA-3 checksum of message <MESSAGE>.\n"
           "  -f <FILE>                 Print the SHA-3 checksum of file <FILE>.\n"
           "  -d [224|256|384|512]      Output size in bits (default is 256).\n"
           "  --help                    Display this information.\n",
           argv0);
    exit(0);
}

int main(int argc, char* argv[])
{
    Sha3::SHA3DigestSize size = Sha3::SHA3_256;
    int isFile = -1;
    std::string input = "";

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-m"))
        {
            if (i + 1 < argc)
            {
                input = argv[i + 1];
                isFile = 0;
                i++;
            }
            else
                showHelp(argv[0]);
        }
        else if (!strcmp(argv[i], "-f"))
        {
            if (i + 1 < argc)
            {
                input = argv[i + 1];
                isFile = 1;
                i++;
            }
            else
                showHelp(argv[0]);
        }
        else if (!strcmp(argv[i], "-d"))
        {
            if (i + 1 < argc)
            {
                if (!strcmp(argv[i + 1], "224"))
                    size = Sha3::SHA3_224;
                else if (!strcmp(argv[i + 1], "256"))
                    size = Sha3::SHA3_256;
                else if (!strcmp(argv[i + 1], "384"))
                    size = Sha3::SHA3_384;
                else if (!strcmp(argv[i + 1], "512"))
                    size = Sha3::SHA3_512;
                else
                    showHelp(argv[0]);
                i++;
            }
            else
                showHelp(argv[0]);
        }
        else if (!strcmp(argv[i], "--help"))
            showHelp(argv[0]);
    }
    if (isFile < 0)
        showHelp(argv[0]);

    Sha3 sha3(size);

    ByteArray message = isFile == 1 ? ByteArray::fromFile(input.c_str()) : ByteArray(input.c_str());
    ByteArray digest = sha3.hash(message);

    if (digest.isError())
        printf("ERROR!\n");
    else
        printf("%s: %s\n", sha3.algorithmName().c_str(), digest.toHexString().c_str());

    return 0;
}
