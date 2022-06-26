/**
 * @file md2.c
 * @author Łukasz Grudnik (https://github.com/lukaszgrudnik)
 * @brief MD2 algorithm based on RFC documentation https://datatracker.ietf.org/doc/html/rfc1319
 * @version 0.1
 * @date 2022-06-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char S[256] =
    {
        41, 46, 67, 201, 162, 216, 124, 1, 61, 54, 84, 161, 236, 240, 6,
        19, 98, 167, 5, 243, 192, 199, 115, 140, 152, 147, 43, 217, 188,
        76, 130, 202, 30, 155, 87, 60, 253, 212, 224, 22, 103, 66, 111, 24,
        138, 23, 229, 18, 190, 78, 196, 214, 218, 158, 222, 73, 160, 251,
        245, 142, 187, 47, 238, 122, 169, 104, 121, 145, 21, 178, 7, 63,
        148, 194, 16, 137, 11, 34, 95, 33, 128, 127, 93, 154, 90, 144, 50,
        39, 53, 62, 204, 231, 191, 247, 151, 3, 255, 25, 48, 179, 72, 165,
        181, 209, 215, 94, 146, 42, 172, 86, 170, 198, 79, 184, 56, 210,
        150, 164, 125, 182, 118, 252, 107, 226, 156, 116, 4, 241, 69, 157,
        112, 89, 100, 113, 135, 32, 134, 91, 207, 101, 230, 45, 168, 2, 27,
        96, 37, 173, 174, 176, 185, 246, 28, 70, 97, 105, 52, 64, 126, 15,
        85, 71, 163, 35, 221, 81, 175, 58, 195, 92, 249, 206, 186, 197,
        234, 38, 44, 83, 13, 110, 133, 40, 132, 9, 211, 223, 205, 244, 65,
        129, 77, 82, 106, 220, 55, 200, 108, 193, 171, 250, 36, 225, 123,
        8, 12, 189, 177, 74, 120, 136, 149, 139, 227, 99, 232, 109, 233,
        203, 213, 254, 59, 0, 29, 57, 242, 239, 183, 14, 102, 88, 208, 228,
        166, 119, 114, 248, 235, 117, 75, 10, 49, 68, 80, 180, 143, 237,
        31, 26, 219, 153, 141, 51, 159, 17, 131, 20};

static unsigned char P[] = {'\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007', '\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017', '\020'};

int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        printf("error: Type message to hash\n");
        printf("./a.out <message>\n");
        return -1;
    }

    // Step 1. Append Padding Bytes

    int p, N, n;

    n = strlen(argv[1]) * sizeof(unsigned char);
    p = 16 - n % 16;
    N = n + p;

    unsigned char *M = (unsigned char *)malloc(N);

    memset(M, P[p], N);
    memcpy(M, argv[1], n);

    // Step 2. Append Checksum

    unsigned char C[16];
    memset(C, 0, 16);

    unsigned char c;
    unsigned char L = 0;

    for (int i = 0; i <= N / 16 - 1; i++)
    {
        for (int j = 0; j <= 15; j++)
        {
            c = M[i * 16 + j];
            C[j] ^= S[c ^ L];
            L = C[j];
        }
    }

    int N_ = N + 16;
    unsigned char *M_ = (unsigned char *)malloc(N_);

    memcpy(M_, M, N);
    memcpy(M_ + N, C, 16);

    //  Step 3. Initialize MD Buffer

    unsigned char X[48] = {0};

    //  Step 4. Process Message in 16-Byte Blocks

    unsigned int t = 0;

    for (int m = 0; m < 16; m++)
    {
        printf("%d ", M_[N + m]);
    }

    for (int i = 0; i <= (N_ / 16) - 1; i++)
    {
        for (int j = 0; j <= 15; j++)
        {
            X[16 + j] = M_[16 * i + j];
            X[32 + j] = (X[16 + j] ^ X[j]);
        }

        t = 0;
        for (int j = 0; j <= 17; j++)
        {
            for (int k = 0; k <= 47; k++)
            {
                t = X[k] ^= S[t];
            }
            t = (t + j) % 256;
        }
    }

    // Step 5. Output

    printf("\nHash:\n");
    for (int i = 0; i < 48; i++)
    {
        printf("%02x ", X[i]);
    }
}
