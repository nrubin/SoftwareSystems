/*

    Copyright 2007 Allen B. Downey

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    The GNU General Public License is available from
    http://www.gnu.org/licenses/.

*/
// #include <stdio.h>
// #include <stdint.h>
// #define RAND_MAX 2147483647.0

float my_random_float()
{
    int x, exp, mant;
    float f;

    // this union is for assembling the float.
    union
    {
        float f;
        int i;
    } b;

    // generate 31 random bits (assuming that RAND_MAX is 2^31 - 1
    x = random();

    // use bit-scan-forward to find the first set bit and
    // compute the exponent
    asm ("bsfl %1, %0"
         :"=r"(exp)
         :"r"(x)
        );
    exp = 126 - exp;

    // use the other 23 bits for the mantissa (for small numbers
    // this means we are re-using some bits)
    mant = x >> 8;
    b.i = (exp << 23) | mant;

    return b.f;
}

// alternative implementation of my algorithm that doesn't use
// embedded assembly
float my_random_float2()
{
    int x;
    int mant;
    int exp = 126;
    int mask = 1;

    union
    {
        float f;
        int i;
    } b;

    // generate random bits until we see the first set bit
    while (1)
    {
        x = random();
        if (x == 0)
        {
            exp -= 31;
        }
        else
        {
            break;
        }
    }

    // find the location of the first set bit and compute the exponent
    while (x & mask)
    {
        mask <<= 1;
        exp--;
    }

    // use the remaining bit as the mantissa
    mant = x >> 8;
    b.i = (exp << 23) | mant;

    return b.f;
}

uint64_t random_long()
{
    uint64_t random_long = 0;
    random_long = random();
    random_long = (random_long << 32) | random();
    return random_long;
}

// compute a random double using my algorithm
double my_random_double()
{
    uint64_t x, mant, exp;
    exp = 1022;
    int mask = 1;

    union
    {
        double d;
        uint64_t i;
    } b;

    // generate random bits until we see the first set bit
    while (1)
    {
        x = random_long();
        if (x == 0)
        {
            exp -= 63;
        }
        else
        {
            break;
        }
    }

    // find the location of the first set bit and compute the exponent
    while (x & mask)
    {
        mask <<= 1;
        exp--;
    }

    // use the remaining bit as the mantissa
    mant = x >> 11;
    b.i = (exp << 52) | mant;

    return b.d;
}

double my_random_double2()
{
    uint64_t x, exp, mant;
    double d;

    // this union is for assembling the float.
    union
    {
        double d;
        uint64_t i;
    } b;

    // generate 31 random bits (assuming that RAND_MAX is 2^31 - 1
    x = random_long();

    // use bit-scan-forward to find the first set bit and
    // compute the exponent
    asm ("bsf %1, %0"
         :"=r"(exp)
         :"r"(x)
        );
    exp = 1022 - exp;

    // use the other 23 bits for the mantissa (for small numbers
    // this means we are re-using some bits)
    mant = x >> 11;
    b.i = (exp << 52) | mant;

    return b.d;
}

// return a constant (this is a dummy function for time trials)
float dummy()
{
    float f = 0.5;

    return f;
}

// generate a random integer and convert to float (dummy function)
float dummy2()
{
    int x;
    float f;

    x = random();
    f = (float) x;

    return f;
}

// generate a random float using the standard algorithm
float random_float()
{
    int x;
    float f;

    x = random();
    f = (float) x / (float)RAND_MAX;

    return f;
}

// generate a random double using the standard algorithm
float random_double()
{
    int x;
    double f;

    x = random();
    f = (double) x / (double)RAND_MAX;

    return f;
}

// int main(int argc, char const *argv[])
// {
//     srandom(16);
//     double l = random_long();
//     double n = my_random_double();
//     printf("%lf\n", n);
//     printf("%lf\n", l);
//     return 0;
// }
