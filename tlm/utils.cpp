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



/*double toDouble(unsigned char *buf)
{
    double val = 0.0;
    val += ((double)buf[0]) * pow(256, 7);
    val += ((double)buf[1]) * pow(256, 6);
    val += ((double)buf[2]) * pow(256, 5);
    val += ((double)buf[3]) * pow(256, 4);
    val += ((double)buf[4]) * pow(256, 3);
    val += ((double)buf[5]) * pow(256, 2);
    val += ((double)buf[6]) * pow(256, 1);
    val += ((double)buf[7]);  
    return val;
} */

/*void fixToChar(unsigned char *c, res_type d)
{
  stringToChar(c,d.to_bin());
}

void stringToChar (unsigned char *buf, string s)
{
  s.erase(0,2); // remove "0b"
  s.erase(FIXED_WIDTH,1); // remove the dot
  char single_char[CHAR_LENGHT];
  for (int i = 0; i < CHARS_AMOUNT; ++i)
    {
      s.copy(single_char,CHAR_LENGHT,i*CHAR_LENGHT); // copy 8 letters (0s and 1s) to char array
      int char_int = stoi(single_char, nullptr, 2); // binary string -> int
      buf[i] = (unsigned char) char_int;
    }
}*/



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

