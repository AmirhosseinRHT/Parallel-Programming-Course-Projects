#include <iostream>
#include <intrin.h>

using namespace std;

union Register
{
    __m128i reg128;
    unsigned char m128_u8[16];
    signed char m128_i8[16];

    unsigned char m128_u16[8];
    signed char m128_i16[8];

    unsigned char m128_u32[4];
    signed char m128_i32[4];

    unsigned char m128_u64[2];
    signed char m128_i64[2];
};


int main ()
{
    return 0;
}