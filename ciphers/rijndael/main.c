#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rijndael.h"

typedef unsigned char ubyte;

void getTestVector(FILE* fp, ubyte* arr, size_t len)
{
    int i = 0;
    int temp;
    char buf[100];

    fscanf(fp, "%s", buf);
    if(strcmp(buf, "COUNT") == 0)
        fscanf(fp, "%*[^=]=");

    if(feof(fp))
        return;

    fscanf(fp, "%*[^=]=");
    for(i = 0; i < len; ++i)
    {
        fscanf(fp, "%2x", &temp);
        arr[i] = (ubyte)temp;
    }
}

int main(int argc, char *argv[])
{
    int i;
    size_t count = 0;
    size_t klen = 0;
    FILE* fp;
    aes_ctx_t* ctx;
    aes_init();

    if(strcmp(argv[1], "--key") == 0)
        klen = atoi(argv[2])/8;

    unsigned char* key = (unsigned char*)malloc(klen);
    unsigned char plain[16] = {0};
    unsigned char cipher[16] = {0};
    unsigned char expected[16] = {0};
    unsigned char iv[16] = {0};

    if(argc >= 4 && strcmp(argv[3], "--test") == 0)
    {
        fp = fopen(argv[4], "r");
        if(!fp)
        {
            perror("ERROR: could not open test file for reading");
            return -1;
        }
        fscanf(fp, "[ENCRYPT]");

        while(!feof(fp))
        {
            getTestVector(fp, key, klen);
            // getTestVector(fp, iv, 16);
            getTestVector(fp, plain, 16);
            getTestVector(fp, expected, 16);
            if(feof(fp))
                break;

            ctx = aes_alloc_ctx(key, klen);
            aes_encrypt(ctx, plain, cipher);

            for(i = 0; i < 16; ++i)
            {
                if(cipher[i] != expected[i])
                {
                    printf("ENCRYPTION ERROR\n");
                    for(i = 0; i < 16; ++i)
                        printf("%.2x", key[i]);
                    printf("\tKEY\n");
                    for(i = 0; i < 16; ++i)
                        printf("%.2x", iv[i]);
                    printf("\tIV\n");
                    for(i = 0; i < 16; ++i)
                        printf("%.2x", plain[i]);
                    printf("\tPLAIN\n");
                    for(i = 0; i < 16; ++i)
                        printf("%.2x", cipher[i]);
                    printf("\tCIPHER\n");
                    for(i = 0; i < 16; ++i)
                        printf("%.2x", expected[i]);
                    printf("\tEXPECTED\n");
                    return -1;
                }
            }
            aes_free_ctx(ctx);
            ctx = aes_alloc_ctx(key, klen);
            aes_decrypt(ctx, cipher, expected);
            for(i = 0; i < 16; ++i)
            {
                if(plain[i] != expected[i])
                {
                    printf("DECRYPTION ERROR\n");
                    for(i = 0; i < 16; ++i)
                        printf("%.2x", key[i]);
                    printf("\tKEY\n");
                    for(i = 0; i < 16; ++i)
                        printf("%.2x", iv[i]);
                    printf("\tIV\n");
                    for(i = 0; i < 16; ++i)
                        printf("%.2x", plain[i]);
                    printf("\tPLAIN\n");
                    for(i = 0; i < 16; ++i)
                        printf("%.2x", cipher[i]);
                    printf("\tCIPHER\n");
                    for(i = 0; i < 16; ++i)
                        printf("%.2x", expected[i]);
                    printf("\tEXPECTED\n");
                    return -1;
                }
            }
            printf("TEST PASSED\n");
            aes_free_ctx(ctx);
        }
        fclose(fp);
    }

    free(key);
    return 0;
}
