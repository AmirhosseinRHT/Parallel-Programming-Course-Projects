#include <stdio.h>
#include <intrin.h>

typedef union {
    __m128i int128;
    unsigned char m128_u8[16];
    signed char m128_i8[16];
    unsigned short m128_u16[8];
    signed short m128_i16[8];
    unsigned int m128_u32[4];
    signed int m128_i32[4];
    unsigned long long m128_u64[2];
    signed long long m128_i64[2];
} Register;

void printIntVec(const Register* vec) {
    printf("Unsigned 8-bit:  ");
    for (int i = 0; i < 16; i++)
        printf("%02X ", vec->m128_u8[i]);
    
    printf("\n");

    printf("Signed 8-bit:    ");
    for (int i = 0; i < 16; i++) 
        printf("%d ", vec->m128_i8[i]);
    
    printf("\n");

    printf("Unsigned 16-bit: ");
    for (int i = 0; i < 8; i++) 
        printf("%04X ", vec->m128_u16[i]);

    printf("\n");

    printf("Signed 16-bit:   ");
    for (int i = 0; i < 8; i++)
        printf("%d ", vec->m128_i16[i]);

    printf("\n");

    printf("Unsigned 32-bit: ");
    for (int i = 0; i < 4; i++)
        printf("%08X ", vec->m128_u32[i]);

    printf("\n");

    printf("Signed 32-bit:   ");
    for (int i = 0; i < 4; i++) 
        printf("%d ", vec->m128_i32[i]);

    printf("\n");

    printf("Unsigned 64-bit: ");
    for (int i = 0; i < 2; i++) 
        printf("%016llX ", vec->m128_u64[i]);

    printf("\n");

    printf("Signed 64-bit:   ");
    for (int i = 0; i < 2; i++) 
        printf("%lld ", vec->m128_i64[i]);

    printf("\n");
}

int main() {

    Register reg;
    unsigned char signedArray[16] = {
        0x80, 0xFF, 0x7F, 0x00, 0x01, 0x02, 0x03, 0x04,
        0xFC, 0xFD, 0xFE, 0xFF, 0x00, 0x01, 0x02, 0x03
    };

    reg.int128 = _mm_loadu_si128((__m128i*)signedArray);
    printf("\nOutput:\n");
    printIntVec(&reg);
    return 0;
}