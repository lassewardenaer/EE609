#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}


int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

int main (void)
{
    FILE *fd = fopen("English_words_list.txt", "r");

    /*
     * Set up the key and iv. Do I need to say to not hard code these in a
     * real application? :-)
     */

    /* A 256 bit key */
    unsigned char *key = malloc(sizeof(char)*16);

    /* A 128 bit IV */
    unsigned char *iv = malloc(sizeof(unsigned char)*8);
    for (int i = 0; i < 8; i++)
    {
        iv[i] = 0x0;
    }
    

    /* Message to be encrypted */
    unsigned char *plaintext =
        (unsigned char *)"This is a top secret";

    /*
     * Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, depending on the
     * algorithm and mode.
     */
    unsigned char ciphertext[32];
    ciphertext[0] = 0x8d;
    ciphertext[1] = 0x20;
    ciphertext[2] = 0xe5;
    ciphertext[3] = 0x05;
    ciphertext[4] = 0x6a;
    ciphertext[5] = 0x8d;
    ciphertext[6] = 0x24;
    ciphertext[7] = 0xd0;
    ciphertext[8] = 0x46;
    ciphertext[9] = 0x2c;
    ciphertext[10] = 0xe7;
    ciphertext[11] = 0x4e;
    ciphertext[12] = 0x49;
    ciphertext[13] = 0x04;
    ciphertext[14] = 0xc1;
    ciphertext[15] = 0xb5;
    ciphertext[16] = 0x13;
    ciphertext[17] = 0xe1;
    ciphertext[18] = 0x0d;
    ciphertext[19] = 0x1d;
    ciphertext[20] = 0xf4;
    ciphertext[21] = 0xa2;
    ciphertext[22] = 0xef;
    ciphertext[23] = 0x2a;
    ciphertext[24] = 0xd4;
    ciphertext[25] = 0x54;
    ciphertext[26] = 0x0f;
    ciphertext[27] = 0xae;
    ciphertext[28] = 0x1c;
    ciphertext[29] = 0xa0;
    ciphertext[30] = 0xaa;
    ciphertext[31] = 0xf9;
    

    /* Buffer for the decrypted text */
    unsigned char decryptedtext[32];

    int decryptedtext_len = 32;
    int ciphertext_len = 32;

    // Get key from file
    for (int i = 0; i < 16; i++)
    {
        key[i] = 0x20;
    }
    int equal = 0;
    int iterations = 0;
    while(fgets(key, 16, fd)!= NULL) /* read a line from a file */ {
        
        int tooShort = 0;
        for (unsigned int i = 0; i < 16; i++)
        {   
            printf("%x", key[i]);
            if (key[i] == 0x0a || tooShort)
            {   
                tooShort = 1;
                key[i] = 0x20;
            }
        }
        key[16] = '\0';
        printf("\n");
        /* Decrypt the ciphertext */
        decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);
        
        int counter = 0;
        for (int i = 0; i < 21; i++)
        {

            if (decryptedtext[i] == plaintext[i])
            {
                //printf("%x %x ||", decryptedtext[i], decryptedtext[i]);
                counter = counter + 1;
            }
            else {
            if (i > 0) 
            {
                //printf("\n");
            }
                break;
            }
            
        }
        
        if (counter > 20)
        {
            equal = 1;
            break;
        }
        
    }

    if (equal)
    {
        /* Show the decrypted text */
        printf("Decrypted text is:\n");
        printf("%s\n", decryptedtext);
    }

        /* Do something useful with the ciphertext here */
    printf("Ciphertext is:\n");
    BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

    printf("%s\n", decryptedtext);
    fclose(fd);

    return 0;
}
