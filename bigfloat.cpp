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
    for (char i = 31; i >= 0; i--)
    {
      std::cout << std::bitset<8>(this->_bytes[i]);
    }
    std::cout << "\n";
  }
  bigFloat operator+(const bigFloat &a)
  {
    //Get this exponent
    long int thisExponent = 0;
    char eTwoCount = 0;
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
        thisExponent += (bit * pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //denormalize
    thisExponent -= 262143;
    //Get exponent of the a
    long int aExponent = 0;
    eTwoCount = 0;
    for (unsigned char i = 29; i < 32; i++)
    {
      for (unsigned char j = 0; j < 8; j++)
      {
        if (i == 29 && j < 4)
          continue;
        if (i == 31 && j == 7)
          break;
        unsigned char mask = 1 << j;
        unsigned char bit = a._bytes[i] & mask;
        bit = bit >> j;
        aExponent += (bit * pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }

    //- bias
    aExponent -= 262143;
    if (aExponent == thisExponent)
    {
      unsigned char newMantisa[30];
      unsigned char carry = 0;
      unsigned char bit = 0;
      for (unsigned char i = 0; i < 30; i++)
      {
        for (unsigned char j = 0; j < 8; j++)
        {
          if (i == 29 && j > 3)
          {
            bit = 1 << j;
            newMantisa[i] = newMantisa[i] & (~bit);
          }
          else
          {
            unsigned char mask = 1 << j; //mascara
            unsigned char aMasked = a._bytes[i] & mask;
            unsigned char tMasked = this->_bytes[i] & mask;
            unsigned char aBit = aMasked >> j;
            unsigned char tBit = tMasked >> j;
            bit = ((aBit ^ tBit) ^ carry);
            carry = ((aBit & tBit) | (aBit & carry)) | (tBit & carry);
            if (bit == 1)
            {
              //put the bit into newMantisa
              newMantisa[i] = newMantisa[i] | (bit << j);
            }
            else
            {
              bit = 1 << j;
              //put the bit into newMantisa
              newMantisa[i] = newMantisa[i] & (~bit);
            }
          }
        }
      }
      //continue sum
      bit = ((1 ^ 1) ^ carry);
      carry = ((1 & 1) | (1 & carry)) | (1 & carry);
      if (bit == 1)
      {
        unsigned char mask = 1 << 4;
        newMantisa[29] = newMantisa[29] | mask;
        RigthShiftMantisa(newMantisa, 1);
        thisExponent++;
      }
      else
      {
        RigthShiftMantisa(newMantisa, 1);
        thisExponent++;
      }
      //Return Value
      bigFloat b = 0;
      ChangeMantisa(b, newMantisa);
      thisExponent += 262143;
      ChangeExponent(b, thisExponent);
      for (char i = 31; i >= 0; i--)
      {
        std::cout << std::bitset<8>(b._bytes[i]);
      }
      std::cout << "\n";
      return b;
    }
  }
  bigFloat operator=(const bigFloat &a)
  {
    for (unsigned char i = 0; i < 32; i++)
    {
      this->_bytes[i] = a._bytes[i];
    }
  }
  //Out value
  double out()
  {
    double o = 0;
    //Get exponent
    long int exponent = 0;
    char eTwoCount = 0;
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
        exponent += (bit * pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //denormalize
    exponent -= 262143;
    //Get mantisa
    double mantisa = 0;
    eTwoCount = -52;
    for (unsigned char i = 23; i < 30; i++)
    {
      for (unsigned char j = 0; j < 8; j++)
      {
        if (i == 29 && j > 3)
          break;
        unsigned char mask = 1 << j;
        unsigned bit = this->_bytes[i] & mask;
        bit = bit >> j;
        mantisa += (bit * pow((double)2, (int)eTwoCount));
        eTwoCount++;
      }
    }
    o = (1 + mantisa) * pow((int)2, (unsigned int)exponent);
    //signal
    unsigned char mask = 1 << 7;
    unsigned char bit = this->_bytes[31] & mask;
    bit = bit >> 7;
    if (bit == 1)
    {
      o *= -1;
    }
    return o;
  }

private:
  //change mantisa
  void ChangeMantisa(bigFloat &a, unsigned char *mantisa)
  {
    for (unsigned char i = 0; i < 30; i++)
    {
      if (i == 29)
      {
        for (unsigned char j = 0; j < 4; j++)
        {
          unsigned char mask = 1 << j;
          unsigned char masked = mask & mantisa[i];
          masked = masked >> j;
          if (masked == 1)
          {
            a._bytes[i] = a._bytes[i] | mask;
          }
          else
          {
            a._bytes[i] = a._bytes[i] & (~mask);
          }
        }
      }
      a._bytes[i] = mantisa[i];
    }
  }
  //chage exponent
  void ChangeExponent(bigFloat &a, long int exponent)
  {
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
        unsigned int masked = exponent & mask;
        if (i == 29)
        {
          masked = masked << 4;
          if (masked != 0)
          {
            a._bytes[i] = a._bytes[i] | masked;
          }
          else
          {
            masked = 1 << j;
            a._bytes[i] = a._bytes[i] & (~masked);
          }
        }
        else if (i == 30)
        {
          masked = masked >> 4;
          if (masked != 0)
          {
            a._bytes[i] = a._bytes[i] | masked;
          }
          else
          {
            masked = 1 << j;
            a._bytes[i] = a._bytes[i] & (~masked);
          }
        }
        else
        {
          masked = masked >> 12;
          if (masked != 0)
          {
            a._bytes[i] = a._bytes[i] | masked;
          }
          else
          {
            masked = 1 << j;
            a._bytes[i] = a._bytes[i] & (~masked);
          }
        }
        intBitcount++;
      }
    }
  }
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
  //rigth shift
  void RigthShiftMantisa(unsigned char *mantisa, unsigned int a)
  {
    unsigned char *b = mantisa;
    unsigned char c[30];
    for (unsigned char i = 0; i < 30; i++)
    {
      c[i] = 0;
    }
    for (unsigned char i = 0; i < 30; i++)
    {
      for (unsigned char j = 0; j < 8; j++)
      {
        unsigned char mask = 1 << j;
        unsigned char masked = mask & b[i];
        unsigned char bit = masked >> j;
        char minId = j - a;
        if (minId > 0)
        {
          masked = masked >> a;
          c[i] = masked | c[i];
        }
        else if (bit == 1)
        {
          char bitId = (((a - j) % 8) - 8) * -1;
          bitId = (bitId == 8) ? 0 : bitId;
          int byteId = ((j + (i * 8)) - (bitId + a)) / 8;
          if (byteId < 30 && byteId >= 0)
          {
            masked = 1 << bitId;
            c[byteId] = masked | c[byteId];
          }
        }
      }
    }
    for (unsigned char i = 0; i < 30; i++)
    {
      mantisa[i] = c[i];
    }
  }
  //left shift
  void LeftShiftMantisa(unsigned char *mantisa, unsigned int a)
  {
    unsigned char *b = mantisa;
    unsigned char c[30];
    for (unsigned char i = 0; i < 30; i++)
    {
      c[i] = 0;
    }
    for (unsigned char i = 0; i < 30; i++)
    {
      for (unsigned char j = 0; j < 8; j++)
      {
        unsigned char mask = 1 << j;
        unsigned char masked = mask & b[i];
        unsigned char bit = masked >> j;
        if ((a + j) < 8)
        {
          masked = masked << a;
          c[i] = masked | c[i];
        }
        else if (bit == 1)
        {
          char bitId = (a + j) % 8;
          int byteId = ((a + (j + (i * 8))) - bitId) / 8;
          if (byteId < 30)
          {
            masked = 1 << bitId;
            c[byteId] = masked | c[byteId];
          }
        }
      }
    }
    for (unsigned char i = 0; i < 30; i++)
    {
      mantisa[i] = c[i];
    }
  }
  unsigned char _bytes[32];
};
using namespace std;
int main()
{
  bigFloat f = 3.52;
  bigFloat g = 2.25;
  f = f + g;
  double d = f.out();
  cout << d << "\n";
  return 0;
}
