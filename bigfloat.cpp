#include <iostream>
#include <thread>
#include <cmath>
#include <cfloat>
#include <string.h>
#include <stdio.h>
#include <bitset>

class bigFloat
{
public:
  bigFloat(const float &a)
  {
    for (unsigned char i = 0; i < 32; i++)
    {
      this->_bytes[i] = 0;
    }
    //Pega as partes do numero flutuante
    typedef union {
      float f;
      struct
      {
        unsigned int mantisa : 23;
        unsigned int exponent : 8;
        unsigned int sign : 1;
      } parts;
    } float_cast;
    float_cast d1 = {.f = a};
    //Set whether the number is positive
    if (d1.parts.sign == 1)
    {
      unsigned char mask = 1 << 7;
      this->_bytes[31] = this->_bytes[31] | mask;
    }
    //Adjust the bias of the exponent float = 127 | octuple = 262143
    //so float + 262016 to normalize exponent
    unsigned int exNormalized = d1.parts.exponent + 262016;
    //Save new exponent
    unsigned char intBitcount = 0;
    for (unsigned char i = 29; i < 32; i++)
    {
      for (unsigned char j = 0; j < 8 && intBitcount < 19; j++)
      {
        //If mantisa
        if (i == 29 && j < 4)
          continue;
        unsigned int mask = 1 << intBitcount;
        unsigned int masked = exNormalized & mask;
        if (i == 29)
        {
          masked = masked << 4;
          this->_bytes[i] = this->_bytes[i] | masked;
        }
        else if (i == 30)
        {
          masked = masked >> 4;
          this->_bytes[i] = this->_bytes[i] | masked;
        }
        else
        {
          masked = masked >> 12;
          this->_bytes[i] = this->_bytes[i] | masked;
        }
        intBitcount++;
      }
    }
    //for mantisa
    char mantisaCount = 22;
    for (unsigned char i = 29; i >= 26; i--)
    {
      for (char j = 7; j >= 0 && mantisaCount >= 0; j--)
      {
        if (i == 29 && j > 3)
          continue;
        unsigned int mask = 1 << mantisaCount;
        unsigned int masked = d1.parts.mantisa & mask;
        if (i == 29)
        {
          masked = masked >> 19;
        }
        else if (i == 28)
        {
          masked = masked >> 11;
        }
        else if (i == 27)
        {
          masked = masked >> 3;
        }
        else if (i == 26)
        {
          masked = masked >> 5;
        }
        this->_bytes[i] = masked | this->_bytes[i];
        mantisaCount--;
      }
    }
    std::cout << std::bitset<8>(this->_bytes[31]) << std::bitset<8>(this->_bytes[30]) << std::bitset<8>(this->_bytes[29]) << std::bitset<8>(this->_bytes[28]) << std::bitset<8>(this->_bytes[27]) << std::bitset<8>(this->_bytes[26]) << std::bitset<8>(this->_bytes[25]) << std::bitset<8>(this->_bytes[25]) << std::bitset<8>(this->_bytes[24]) << std::bitset<8>(this->_bytes[23]) << std::bitset<8>(this->_bytes[22]) << std::bitset<8>(this->_bytes[21]) << std::bitset<8>(this->_bytes[20]) << "\n";
  }
  double out()
  {
    double o = 0;
    //Get exponent
    long int exponent = 0;
    long long int eTwoCount = 0;
    for (unsigned char i = 29; i < 32; i++)
    {
      for (unsigned char j = 0; j < 8; j++)
      {
        if (i == 29 && j < 4)
          continue;
        if (i == 31 && j == 7)
          break;
        unsigned char mask = 1 << j;
        unsigned char bit = this->_bytes[i] & mask;
        bit = bit >> j;
        exponent += (bit * pow((int)2, eTwoCount));
        eTwoCount++;
      }
    }
    //denormalize
    exponent -= 262143;
    o = pow((double)4, (int)-8);

    return o;
  }

private:
  //for ints
  long long int pow(int n, unsigned int e)
  {
    if (e == 0)
    {
      return 1;
    }
    long long int p = n;
    for (int i = 1; i < e; i++)
    {
      p *= n;
    }
    return p;
  }
  //for floats
  double pow(double n, int e)
  {
    if (e == 0)
    {
      return 1;
    }
    else if (e > 0)
    {
      double p = n;
      for (int i = 1; i < e; i++)
      {
        p *= n;
      }
      return p;
    }
    else
    {
      double p = 1 / n;
      for (int i = 1; i < (e * -1); i++)
      {
        p *= 1 / n;
      }
      return p;
    }
  }
  unsigned char _bytes[32];
};
using namespace std;
int main()
{
  bigFloat f = -5.25;
  double d = f.out();
  cout << d << "\n";
  return 0;
}
