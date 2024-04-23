#include "utils.hpp"
#include <iostream>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <string>


int toInt(unsigned char *buf)
{
    int val = 0;
    val += ((int)buf[0]) << 24;
    val += ((int)buf[1]) << 16;
    val += ((int)buf[2]) << 8;
    val += ((int)buf[3]);
    return val;
}

num_f toNum_f(unsigned char *buf)
{
    uint64_t val = 0;

    val += (uint64_t)(buf[0]) << 8;
    val += (uint64_t)(buf[1]) << 16;
    val += (uint64_t)(buf[2]) << 24;
    val += (uint64_t)(buf[3]) << 32;
    val += (uint64_t)(buf[4]) << 40;
    val += buf[5];

    return ((double)val / 1073741824.0); // 1073741824 = 2^30
}


void intToUchar(unsigned char *buf,int val)
{
    buf[0] = (char) (val >> 24);
    buf[1] = (char) (val >> 16);
    buf[2] = (char) (val >> 8);
    buf[3] = (char) (val);
}


void doubleToUchar(unsigned char *buf, double val)
{
    // Koristimo memcpy kako bismo sigurno kopirali memoriju iz double u uint64_t
    uint64_t intVal;
    static_assert(sizeof(intVal) == sizeof(val), "Size mismatch between double and uint64_t");
    std::memcpy(&intVal, &val, sizeof(val));

    // Kopiramo svaki bajt uint64_t-a u unsigned char niz
    for (int i = 0; i < sizeof(uint64_t); ++i)
    {
        buf[i] = static_cast<unsigned char>((intVal >> (i * 8)) & 0xFF);
    }
}


double toDouble(unsigned char *buf)
{
    // Kreiramo 64-bitni integer i postavljamo ga na nulu
    uint64_t intVal = 0;

    // Kopiramo svaki bajt iz unsigned char niza u uint64_t
    for (int i = 0; i < sizeof(uint64_t); ++i)
    {
        intVal |= static_cast<uint64_t>(buf[i]) << (i * 8);
    }

    // Koristimo memcpy kako bismo sigurno kopirali memoriju iz uint64_t u double
    double val;
    static_assert(sizeof(intVal) == sizeof(val), "Size mismatch between double and uint64_t");
    std::memcpy(&val, &intVal, sizeof(val));

    return val;
}

