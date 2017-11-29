#include <iostream>
#include <bitset>

class bigFloat
{
public:
  bigFloat() {}
  bigFloat(const float &a)
  {
    for (unsigned char i = 0; i < 32; i++)
    {
      this->_bytes[i] = 0;
    }
    if (a == 0)
    {
      return;
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
  } /*
  bigFloat(const float &a)
  {
    for (unsigned char i = 0; i < 32; i++)
    {
      this->_bytes[i] = 0;
    }
    //Pega as partes do numero flutuante
    typedef union {
      double d;
      struct
      {
        unsigned int mantisa : 23;
        unsigned int exponent : 8;
        unsigned int sign : 1;
      } parts;
    } double_cast;
    double_cast d1 = {.d = a};
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
  }*/
  static bigFloat Pi()
  {
    bigFloat pi;
    pi._bytes[31] = 64;
    pi._bytes[30] = 0;
    pi._bytes[29] = 9;
    pi._bytes[28] = 33;
    pi._bytes[27] = 251;
    pi._bytes[26] = 84;
    pi._bytes[25] = 68;
    pi._bytes[24] = 45;
    pi._bytes[23] = 24;
    pi._bytes[22] = 70;
    pi._bytes[21] = 152;
    pi._bytes[20] = 152;
    pi._bytes[19] = 204;
    pi._bytes[18] = 21;
    pi._bytes[17] = 112;
    pi._bytes[16] = 27;
    pi._bytes[15] = 131;
    pi._bytes[14] = 154;
    pi._bytes[13] = 37;
    pi._bytes[12] = 32;
    pi._bytes[11] = 73;
    pi._bytes[10] = 193;
    pi._bytes[9] = 17;
    pi._bytes[8] = 76;
    pi._bytes[7] = 249;
    pi._bytes[6] = 142;
    pi._bytes[5] = 128;
    pi._bytes[4] = 65;
    pi._bytes[3] = 119;
    pi._bytes[2] = 212;
    pi._bytes[1] = 199;
    pi._bytes[0] = 100;
    return pi;
  }
  bigFloat operator-(const bigFloat &a)
  {
    bool negative = false;
    bigFloat nA = 0;
    bigFloat nB = 0;
    {
      //Get signal of the numbers
      unsigned char mask = 1 << 7;
      unsigned char thisSignal = mask & this->_bytes[31];
      thisSignal = thisSignal >> 7;
      unsigned char aSignal = mask & a._bytes[31];
      aSignal = aSignal >> 7;
      if (thisSignal == 0 && aSignal == 0 && (a <= *this))
      {
        nA = *this;
        nB = a;
      }
      else if (thisSignal == 0 && aSignal == 0 && (a > *this))
      {
        nB = *this;
        nA = a;
        negative = true;
      }
      else if (thisSignal == 0 && aSignal == 1)
      {
        nA = *this;
        nB = a;
        unsigned char mask = 1 << 7;
        nB._bytes[31] = nB._bytes[31] & (~mask);
        nB = nA + nB;
        return nB;
      }
      else if (thisSignal == 1 && aSignal == 0)
      {
        nA = *this;
        nB = a;
        unsigned char mask = 1 << 7;
        nA._bytes[31] = nA._bytes[31] & (~mask);
        nB = nA + nB;
        nB._bytes[31] = nB._bytes[31] | mask;
        return nB;
      }
      else if (thisSignal == 1 && aSignal == 1 && (a < *this))
      {
        nA = *this;
        nB = a;
        unsigned char mask = 1 << 7;
        nA._bytes[31] = nA._bytes[31] & (~mask);
        nB._bytes[31] = nB._bytes[31] & (~mask);
        negative = true;
      }
      else if (thisSignal == 1 && aSignal == 1 && (a > *this))
      {
        nA = a;
        nB = *this;
        unsigned char mask = 1 << 7;
        nA._bytes[31] = nA._bytes[31] & (~mask);
        nB._bytes[31] = nB._bytes[31] & (~mask);
      }
    }
    //Get this exponent
    long int nAExponent = 0;
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
        unsigned char bit = nA._bytes[i] & mask;
        bit = bit >> j;
        nAExponent += (bit * pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //denormalize
    nAExponent -= 262143;
    //Get exponent of the a
    long int nBExponent = 0;
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
        unsigned char bit = nB._bytes[i] & mask;
        bit = bit >> j;
        nBExponent += (bit * pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //- bias
    nBExponent -= 262143;
    //Get Mantisas
    unsigned char thisMantisa[30];
    unsigned char aMantisa[30];
    for (unsigned char i = 0; i < 30; i++)
    {
      if (i == 29)
      {
        for (unsigned char j = 0; j < 8; j++)
        {
          if (j > 3)
          {
            unsigned char mask = 1 << j;
            thisMantisa[i] = thisMantisa[i] & (~mask);
            aMantisa[i] = aMantisa[i] & (~mask);
          }
          else
          {
            unsigned char mask = 1 << j;
            unsigned char thisMasked = nA._bytes[i] & mask;
            unsigned char aMasked = nB._bytes[i] & mask;
            if (thisMasked != 0)
            {
              thisMantisa[i] = thisMantisa[i] | thisMasked;
            }
            else
            {
              thisMasked = 1 << j;
              thisMantisa[i] = thisMantisa[i] & (~thisMasked);
            }
            if (aMasked != 0)
            {
              aMantisa[i] = aMantisa[i] | aMasked;
            }
            else
            {
              aMasked = 1 << j;
              aMantisa[i] = aMantisa[i] & (~aMasked);
            }
          }
        }
      }
      else
      {
        thisMantisa[i] = nA._bytes[i];
        aMantisa[i] = nB._bytes[i];
      }
    }
    bool bExponent = false;
    //Ajusta o expoente 1,01010 => 0,101010

    RightShiftMantisa(aMantisa, 1);
    RightShiftMantisa(thisMantisa, 1);
    unsigned char mask = 1 << 3;
    aMantisa[29] = aMantisa[29] | mask;
    thisMantisa[29] = thisMantisa[29] | mask;
    nBExponent++;
    nAExponent++;

    if (nBExponent != nAExponent)
    {
      bExponent = true;
      if (nBExponent > nAExponent)
      {
        int p = (nBExponent - nAExponent);
        RightShiftMantisa(thisMantisa, p);
        nAExponent = nBExponent;
      }
      else
      {
        int p = (nAExponent - nBExponent);
        RightShiftMantisa(aMantisa, p);
        nBExponent = nAExponent;
      }
    }
    //subtract
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
          continue;
        }
        unsigned char mask = 1 << j; //mascara
        unsigned char thisMasked = thisMantisa[i] & mask;
        unsigned char aMasked = aMantisa[i] & mask;
        thisMasked = thisMasked >> j;
        aMasked = aMasked >> j;
        bit = (thisMasked ^ aMasked ^ carry);
        carry = ~thisMasked & carry | ~thisMasked & aMasked | aMasked & carry;
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
    if (bExponent == false)
    {
      //continue subtract
      bit = 1 ^ 1 ^ carry;
      carry = 1 & carry;
      if (bit == 1 && carry == 1)
      {
        unsigned char mask = 1 << 4;
        newMantisa[29] = newMantisa[29] | mask;
        RightShiftMantisa(newMantisa, 1);
        nAExponent++;
      }
      else if (bit == 0 && carry == 1)
      {
        RightShiftMantisa(newMantisa, 1);
        nAExponent++;
      }
      else if (bit == 0 && carry == 0)
      {
        unsigned int count = 1;
        bool broke = false;
        for (char i = 29; i >= 0 && broke == false; i--)
        {
          for (char j = 7; j >= 0 && broke == false; j--)
          {
            if (i == 29 && j > 3)
              continue;
            unsigned char mask = 1 << j;
            unsigned char masked = newMantisa[i] & mask;
            masked = masked >> j;
            if (masked == 1)
            {
              LeftShiftMantisa(newMantisa, count);
              nAExponent = nAExponent - count;
              broke = true;
            }
            else
            {
              count++;
            }
          }
        }
        if (broke == false)
          nAExponent = -262143;
      }
    }
    else
    {
      if (carry == 1)
      {
        RightShiftMantisa(newMantisa, 1);
        nAExponent++;
      }
      else
      {
        unsigned int count = 1;
        bool broke = false;
        for (char i = 29; i >= 0 && broke == false; i--)
        {
          for (char j = 7; j >= 0 && broke == false; j--)
          {
            if (i == 29 && j > 3)
              continue;
            unsigned char mask = 1 << j;
            unsigned char masked = newMantisa[i] & mask;
            masked = masked >> j;
            if (masked == 1)
            {
              LeftShiftMantisa(newMantisa, count);
              //hidden 1
              masked = 1 << 4;
              newMantisa[29] = newMantisa[29] & (~masked);
              nAExponent = nAExponent - count;
              broke = true;
            }
            else
            {
              count++;
            }
          }
        }
      }
    }
    //Return Value
    bigFloat b = 0;
    ChangeMantisa(b, newMantisa);
    nAExponent += 262143;
    ChangeExponent(b, nAExponent);
    if (negative == true)
    {
      unsigned char mask = 1 << 7;
      b._bytes[31] = b._bytes[31] | mask;
    }
    return b;
  }
  bigFloat operator+(const bigFloat &a)
  {
    bigFloat nA = 0;
    bigFloat nB = 0;
    bool negative = false;
    {
      //Get signal of the numbers
      unsigned char mask = 1 << 7;
      unsigned char thisSignal = mask & this->_bytes[31];
      thisSignal = thisSignal >> 7;
      unsigned char aSignal = mask & a._bytes[31];
      aSignal = aSignal >> 7;
      if (thisSignal == 0 && aSignal == 0)
      {
        nA = *this;
        nB = a;
      }
      else if (thisSignal == 1 && aSignal == 1)
      {
        nA = *this;
        nB = a;
        negative = true;
      }
      else if (thisSignal == 1 || aSignal == 1)
      {
        nB = a;
        nA = *this;
        unsigned char mask = 1 << 7;
        nA._bytes[31] = nA._bytes[31] & (~mask);
        nB._bytes[31] = nB._bytes[31] & (~mask);
        bigFloat nC = nA - nB;
        if (thisSignal == 1 && nA > nB)
        {
          nC._bytes[31] = nC._bytes[31] | mask;
        }
        if (thisSignal == 1 && nA < nB)
        {
          nC._bytes[31] = nC._bytes[31] & ~mask;
        }
        return nC;
      }
    }
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
        unsigned char bit = nA._bytes[i] & mask;
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
        unsigned char bit = nB._bytes[i] & mask;
        bit = bit >> j;
        aExponent += (bit * pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //- bias
    aExponent -= 262143;
    //Get Mantisas
    unsigned char thisMantisa[30];
    unsigned char aMantisa[30];
    for (unsigned char i = 0; i < 30; i++)
    {
      if (i == 29)
      {
        for (unsigned char j = 0; j < 8; j++)
        {
          if (j > 3)
          {
            unsigned char mask = 1 << j;
            thisMantisa[i] = thisMantisa[i] & (~mask);
            aMantisa[i] = aMantisa[i] & (~mask);
          }
          else
          {
            unsigned char mask = 1 << j;
            unsigned char thisMasked = nA._bytes[i] & mask;
            unsigned char aMasked = nB._bytes[i] & mask;
            if (thisMasked != 0)
            {
              thisMantisa[i] = thisMantisa[i] | thisMasked;
            }
            else
            {
              thisMasked = 1 << j;
              thisMantisa[i] = thisMantisa[i] & (~thisMasked);
            }
            if (aMasked != 0)
            {
              aMantisa[i] = aMantisa[i] | aMasked;
            }
            else
            {
              aMasked = 1 << j;
              aMantisa[i] = aMantisa[i] & (~aMasked);
            }
          }
        }
      }
      else
      {
        thisMantisa[i] = nA._bytes[i];
        aMantisa[i] = nB._bytes[i];
      }
    }
    bool bExponent = false;
    if (aExponent != thisExponent)
    {
      bExponent = true;
      if (aExponent > thisExponent)
      {
        int p = (aExponent - thisExponent);
        unsigned char mask = 1 << 4;
        thisMantisa[29] = thisMantisa[29] | mask;
        RightShiftMantisa(thisMantisa, p);
        thisExponent = aExponent;
      }
      else
      {
        int p = (thisExponent - aExponent);
        unsigned char mask = 1 << 4;
        aMantisa[29] = aMantisa[29] | mask;
        RightShiftMantisa(aMantisa, p);
        aExponent = thisExponent;
      }
    }
    //sum
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
          continue;
        }
        unsigned char mask = 1 << j; //mascara
        unsigned char thisMasked = thisMantisa[i] & mask;
        unsigned char aMasked = aMantisa[i] & mask;
        thisMasked = thisMasked >> j;
        aMasked = aMasked >> j;
        bit = ((aMasked ^ thisMasked) ^ carry);
        carry = ((aMasked & thisMasked) | (aMasked & carry)) | (thisMasked & carry);
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
    if (bExponent == false)
    {
      //continue sum
      bit = ((1 ^ 1) ^ carry);
      carry = ((1 & 1) | (1 & carry)) | (1 & carry);
      if (bit == 1)
      {
        unsigned char mask = 1 << 4;
        newMantisa[29] = newMantisa[29] | mask;
        RightShiftMantisa(newMantisa, 1);
        thisExponent++;
      }
      else
      {
        RightShiftMantisa(newMantisa, 1);
        thisExponent++;
      }
    }
    else
    {
      if (carry == 1)
      {
        RightShiftMantisa(newMantisa, 1);
        thisExponent++;
      }
    }
    //Return Value
    bigFloat b = 0;
    ChangeMantisa(b, newMantisa);
    thisExponent += 262143;
    ChangeExponent(b, thisExponent);
    if (negative == true)
    {
      unsigned char mask = 1 << 7;
      b._bytes[31] = b._bytes[31] | mask;
    }
    return b;
  }
  bigFloat operator++(int)
  {
    *this = *this + 1;
  }
  bigFloat operator--(int)
  {
    *this = *this - 1;
  }
  bigFloat operator*(const bigFloat &a)
  {
    bigFloat nA = *this;
    bigFloat nB = a;
    if (nA.out() == 0 | nB.out() == 0)
    {
      nB = (bigFloat)0;
      return nB;
    }
    //ajust sign
    {
      unsigned char mask = 1 << 7;
      unsigned char abit = nA._bytes[31] & mask;
      unsigned char bbit = nB._bytes[31] & mask;
      abit = abit >> 7;
      bbit = bbit >> 7;
      if (abit == 1 && bbit == 1)
      {
        nA._bytes[31] = nA._bytes[31] & (~mask);
      }
      else if (abit == 0 && bbit == 1)
      {
        nA._bytes[31] = nA._bytes[31] | mask;
      }
    }
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
        unsigned char bit = nA._bytes[i] & mask;
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
        unsigned char bit = nB._bytes[i] & mask;
        bit = bit >> j;
        aExponent += (bit * pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //- bias
    aExponent -= 262143;
    //Get Mantisas
    unsigned char thisMantisa[30];
    unsigned char aMantisa[30];
    for (unsigned char i = 0; i < 30; i++)
    {
      if (i == 29)
      {
        for (unsigned char j = 0; j < 8; j++)
        {
          if (j > 3)
          {
            unsigned char mask = 1 << j;
            thisMantisa[i] = thisMantisa[i] & (~mask);
            aMantisa[i] = aMantisa[i] & (~mask);
          }
          else
          {
            unsigned char mask = 1 << j;
            unsigned char thisMasked = nA._bytes[i] & mask;
            unsigned char aMasked = nB._bytes[i] & mask;
            if (thisMasked != 0)
            {
              thisMantisa[i] = thisMantisa[i] | thisMasked;
            }
            else
            {
              thisMasked = 1 << j;
              thisMantisa[i] = thisMantisa[i] & (~thisMasked);
            }
            if (aMasked != 0)
            {
              aMantisa[i] = aMantisa[i] | aMasked;
            }
            else
            {
              aMasked = 1 << j;
              aMantisa[i] = aMantisa[i] & (~aMasked);
            }
          }
        }
      }
      else
      {
        thisMantisa[i] = nA._bytes[i];
        aMantisa[i] = nB._bytes[i];
      }
    }
    {
      //put 1 in 5ºbit 00010000
      unsigned char mask = 1 << 4;
      thisMantisa[29] = thisMantisa[29] | mask;
      aMantisa[29] = aMantisa[29] | mask;
    }
    //multiply the mantissas
    unsigned char result[60];
    {
      unsigned char aMultiply[60];
      for (unsigned char i = 0; i < 60; i++)
      {
        result[i] = 0;
        if (i < 30)
        {
          aMultiply[i] = aMantisa[i];
        }
        else
        {
          aMultiply[i] = 0;
        }
      }
      for (unsigned char i = 0; i < 30; i++)
      {
        for (unsigned char j = 0; j < 8; j++)
        {
          if (i == 29 && j > 4)
            continue;
          unsigned char mask = 1 << j;
          unsigned char thisMasked = thisMantisa[i] & mask;
          if (thisMasked != 0)
          {
            if (i == 0 && j == 0)
            {
              //result <- aMantisa
              for (unsigned char k = 0; k < 30; k++)
              {
                result[k] = aMultiply[k];
              }
            }
            else
            {
              RightShiftMantisa(result, 1);
              //sum result with aMantisa
              unsigned char carry = 0;
              unsigned char bit = 0;
              for (int k = 0; k < 60; k++)
              {
                for (char l = 0; l < 8; l++)
                {
                  mask = 1 << l; //mascara
                  unsigned char aMasked = aMultiply[k] & mask;
                  unsigned char rMasked = result[k] & mask;
                  unsigned char aBit = aMasked >> l;
                  unsigned char rBit = rMasked >> l;
                  bit = ((aBit ^ rBit) ^ carry);
                  carry = ((aBit & rBit) | (aBit & carry)) | (rBit & carry);
                  if (bit == 1)
                  {
                    result[k] = result[k] | mask;
                  }
                  else
                  {
                    result[k] = result[k] & (~mask);
                  }
                }
              }
            }
          }
          else
          {
            RightShiftMantisa(result, 1);
          }
        }
      }
    }
    //normalize result
    int leftZero = -244;
    {
      bool lbreak = false;
      for (char i = 59; i >= 0 && lbreak == false; i--)
      {
        for (char j = 7; j >= 0; j--)
        {
          leftZero++;
          unsigned char mask = 1 << j;
          unsigned char rMasked = result[i] & mask;
          if (rMasked != 0)
          {
            if (leftZero > 0)
              LeftShiftMantisa(result, leftZero);
            else
            {
              RightShiftMantisa(result, leftZero * -1);
            }
            lbreak = true;
            break;
          }
        }
      }
      unsigned mask = 1 << 4;
      result[29] = result[29] & (~mask);
    }
    thisExponent += aExponent;
    thisExponent += (leftZero > 0 ? leftZero : leftZero * -1);
    thisExponent += 262143;
    ChangeMantisa(nA, result);
    ChangeExponent(nA, thisExponent);
    return nA;
  }
  bigFloat operator/(const bigFloat &a)
  {
    bigFloat nA = *this;
    bigFloat nB = a;
    {
      if (this->out() == 0)
      {
        nB = 0;
        return 0;
      }
    }
    //ajust sign
    {
      unsigned char mask = 1 << 7;
      unsigned char abit = nA._bytes[31] & mask;
      unsigned char bbit = nB._bytes[31] & mask;
      abit = abit >> 7;
      bbit = bbit >> 7;
      if (abit == 1 && bbit == 1)
      {
        nA._bytes[31] = nA._bytes[31] & (~mask);
      }
      else if (abit == 0 && bbit == 1)
      {
        nA._bytes[31] = nA._bytes[31] | mask;
      }
    }
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
        unsigned char bit = nA._bytes[i] & mask;
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
        unsigned char bit = nB._bytes[i] & mask;
        bit = bit >> j;
        aExponent += (bit * pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //- bias
    aExponent -= 262143;
    //Get Mantisas
    unsigned char thisMantisa[30];
    unsigned char aMantisa[30];
    for (unsigned char i = 0; i < 30; i++)
    {
      if (i == 29)
      {
        for (unsigned char j = 0; j < 8; j++)
        {
          if (j > 3)
          {
            unsigned char mask = 1 << j;
            thisMantisa[i] = thisMantisa[i] & (~mask);
            aMantisa[i] = aMantisa[i] & (~mask);
          }
          else
          {
            unsigned char mask = 1 << j;
            unsigned char thisMasked = nA._bytes[i] & mask;
            unsigned char aMasked = nB._bytes[i] & mask;
            if (thisMasked != 0)
            {
              thisMantisa[i] = thisMantisa[i] | thisMasked;
            }
            else
            {
              thisMasked = 1 << j;
              thisMantisa[i] = thisMantisa[i] & (~thisMasked);
            }
            if (aMasked != 0)
            {
              aMantisa[i] = aMantisa[i] | aMasked;
            }
            else
            {
              aMasked = 1 << j;
              aMantisa[i] = aMantisa[i] & (~aMasked);
            }
          }
        }
      }
      else
      {
        thisMantisa[i] = nA._bytes[i];
        aMantisa[i] = nB._bytes[i];
      }
    }
    {
      //put 1 in 5ºbit 00010000
      unsigned char mask = 1 << 4;
      thisMantisa[29] = thisMantisa[29] | mask;
      aMantisa[29] = aMantisa[29] | mask;
    }
    //Ajust masntisa to number 0011001000000 -> 0000000011001
    unsigned int nSignifBits = 0;
    for (char i = 0; i < 30; i++)
    {
      for (char j = 0; j < 8; j++)
      {
        unsigned char mask = 1 << j;
        unsigned char aMasked = aMantisa[i] & mask;
        aMasked = aMasked >> j;
        if (aMasked == 1)
        {
          RightShiftMantisa(aMantisa, i * 8 + j);
          nSignifBits = i * 8 + j;
          break;
        }
      }
      if (nSignifBits != 0)
        break;
    }
    if (nSignifBits == 236)
    {
      thisExponent -= aExponent;
      thisExponent += 262143;
      ChangeExponent(nA, thisExponent);
      return nA;
    }
    unsigned char newMantisa[30];
    newMantisa[29] = 0;
    unsigned char rest[30];
    for (unsigned char i = 0; i < 30; i++)
    {
      rest[i] = thisMantisa[i];
    }
    RightShiftMantisa(rest, nSignifBits);
    //count newMantisa bitId
    int newCount = 236;
    //count rest bit
    while (newCount >= 0)
    {
      bool tGreater = false;
      bool broke = false;
      for (char i = 29; i >= 0 && tGreater == false && broke == false; i--)
      {
        for (char j = 7; j >= 0 && tGreater == false; j--)
        {
          if (i == 29 && j > 4)
            continue;
          unsigned char mask = 1 << j;
          unsigned char aMasked = aMantisa[i] & mask;
          unsigned char restMasked = rest[i] & mask;
          aMasked = aMasked >> j;
          restMasked = restMasked >> j;
          if (restMasked > aMasked)
          {
            tGreater = true;
            break;
          }
          else if (restMasked < aMasked)
          {
            broke = true;
            break;
          }
          if (i == 0 && j == 0)
          {
            tGreater = true;
          }
        }
      }
      if (tGreater)
      {
        //subtract rest and aMantisa
        unsigned char carry = 0;
        unsigned char bit = 0;
        for (unsigned char i = 0; i < 30; i++)
        {
          for (unsigned char j = 0; j < 8; j++)
          {
            if (i == 29 && j > 4)
            {
              bit = 1 << j;
              rest[i] = rest[i] & (~bit);
              continue;
            }
            unsigned char mask = 1 << j; //mascara
            unsigned char restMasked = aMantisa[i] & mask;
            unsigned char aMasked = rest[i] & mask;
            restMasked = restMasked >> j;
            aMasked = aMasked >> j;
            bit = (aMasked ^ restMasked ^ carry);
            carry = ~aMasked & carry | ~aMasked & restMasked | restMasked & carry;
            if (bit == 1)
            {
              //put the bit into newMantisa
              rest[i] = rest[i] | (bit << j);
            }
            else
            {
              bit = 1 << j;
              //put the bit into newMantisa
              rest[i] = rest[i] & (~bit);
            }
          }
        }
        char bitId = newCount % 8;
        char byteId = (newCount - bitId) / 8;
        unsigned char mask = 1 << bitId;
        newMantisa[byteId] = newMantisa[byteId] | mask;
        newCount--;
        nSignifBits--;
        bitId = nSignifBits % 8;
        byteId = (nSignifBits - bitId) / 8;
        mask = 1 << bitId;
        LeftShiftMantisa(rest, 1);
        rest[0] = rest[0] | ((thisMantisa[byteId] & mask) >> bitId);
      }
      else
      {
        char bitId = newCount % 8;
        char byteId = (newCount - bitId) / 8;
        unsigned char mask = 1 << bitId;
        newMantisa[byteId] = newMantisa[byteId] & (~mask);
        newCount--;
        nSignifBits--;
        bitId = nSignifBits % 8;
        byteId = (nSignifBits - bitId) / 8;
        mask = 1 << bitId;
        LeftShiftMantisa(rest, 1);
        rest[0] = rest[0] | ((thisMantisa[byteId] & mask) >> bitId);
      }
    }
    int mExponent = 0;
    bool exbreak = false;
    //normalize mantisa
    for (unsigned char i = 29; i >= 0 && exbreak == false; i--)
    {
      for (unsigned char j = 7; j >= 0; j--)
      {
        if (i == 29 && j > 4)
          continue;
        unsigned char mask = 1 << j;
        unsigned char masked = newMantisa[i] & mask;
        if (masked != 0)
        {
          LeftShiftMantisa(newMantisa, mExponent);
          exbreak = true;
          break;
        }
        mExponent++;
      }
    }
    //remove hidden bit
    unsigned char mask = 1 << 4;
    newMantisa[29] = newMantisa[29] & (~mask);
    //newExponent
    thisExponent -= aExponent;
    thisExponent -= mExponent;
    thisExponent += 262143;
    ChangeMantisa(nA, newMantisa);
    ChangeExponent(nA, thisExponent);
    return nA;
  }
  bigFloat operator=(const bigFloat &a)
  {
    for (unsigned char i = 0; i < 32; i++)
    {
      this->_bytes[i] = a._bytes[i];
    }
  }
  friend bool operator==(const bigFloat &b, const bigFloat &a)
  {
    //Get exponent of the a
    long int aExponent = 0;
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
        unsigned char bit = a._bytes[i] & mask;
        bit = bit >> j;
        aExponent += (bit * pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //- bias
    aExponent -= 262143;
    //Get this exponent
    long int bExponent = 0;
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
        unsigned char bit = b._bytes[i] & mask;
        bit = bit >> j;
        bExponent += (bit * bigFloat::pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //denormalize
    bExponent -= 262143;
    if (aExponent != bExponent)
    {
      return false;
    }
    else
    {
      for (char i = 29; i >= 0; i--)
      {
        for (char j = 7; j >= 0; j--)
        {
          if (i == 29 && j > 3)
            continue;
          unsigned char mask = 1 << j;
          unsigned char aMasked = a._bytes[i] & mask;
          unsigned char bMasked = b._bytes[i] & mask;
          aMasked = aMasked >> j;
          bMasked = bMasked >> j;
          if (aMasked != bMasked)
          {
            return false;
          }
        }
      }
    }
    return true;
  }
  friend bool operator!=(const bigFloat &b, const bigFloat &a)
  {
    {
      unsigned char mask = 1 << 7;
      unsigned char amasked = mask & a._bytes[31];
      unsigned char bmasked = mask & b._bytes[31];
      if (amasked != bmasked)
      {
        return true;
      }
    }
    //Get exponent of the a
    long int aExponent = 0;
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
        unsigned char bit = a._bytes[i] & mask;
        bit = bit >> j;
        aExponent += (bit * pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //- bias
    aExponent -= 262143;
    //Get this exponent
    long int bExponent = 0;
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
        unsigned char bit = b._bytes[i] & mask;
        bit = bit >> j;
        bExponent += (bit * bigFloat::pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //denormalize
    bExponent -= 262143;
    if (aExponent != bExponent)
    {
      return true;
    }
    else
    {
      for (char i = 29; i >= 0; i--)
      {
        for (char j = 7; j >= 0; j--)
        {
          if (i == 29 && j > 3)
            continue;
          unsigned char mask = 1 << j;
          unsigned char aMasked = a._bytes[i] & mask;
          unsigned char bMasked = b._bytes[i] & mask;
          aMasked = aMasked >> j;
          bMasked = bMasked >> j;
          if (aMasked != bMasked)
          {
            return true;
          }
        }
      }
    }
    return false;
  }
  friend bool operator<(const bigFloat &b, const bigFloat &a)
  {
    //Get exponent of the a
    long int aExponent = 0;
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
        unsigned char bit = a._bytes[i] & mask;
        bit = bit >> j;
        aExponent += (bit * pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //- bias
    aExponent -= 262143;
    //Get this exponent
    long int bExponent = 0;
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
        unsigned char bit = b._bytes[i] & mask;
        bit = bit >> j;
        bExponent += (bit * bigFloat::pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //denormalize
    bExponent -= 262143;
    if (aExponent > bExponent)
    {
      return true;
    }
    else if (aExponent < bExponent)
    {
      return false;
    }
    else
    {
      for (char i = 29; i >= 0; i--)
      {
        for (char j = 7; j >= 0; j--)
        {
          if (i == 29 && j > 3)
            continue;
          unsigned char mask = 1 << j;
          unsigned char aMasked = a._bytes[i] & mask;
          unsigned char bMasked = b._bytes[i] & mask;
          aMasked = aMasked >> j;
          bMasked = bMasked >> j;
          if (aMasked > bMasked)
          {
            return true;
          }
          else if (bMasked > aMasked)
          {
            return false;
          }
        }
      }
    }
    return false;
  }
  friend bool operator<=(const bigFloat &b, const bigFloat &a)
  {
    //Get exponent of the a
    long int aExponent = 0;
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
        unsigned char bit = a._bytes[i] & mask;
        bit = bit >> j;
        aExponent += (bit * pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //- bias
    aExponent -= 262143;
    //Get this exponent
    long int bExponent = 0;
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
        unsigned char bit = b._bytes[i] & mask;
        bit = bit >> j;
        bExponent += (bit * bigFloat::pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //denormalize
    bExponent -= 262143;
    if (aExponent > bExponent)
    {
      return true;
    }
    else if (aExponent < bExponent)
    {
      return false;
    }
    else
    {
      for (char i = 29; i >= 0; i--)
      {
        for (char j = 7; j >= 0; j--)
        {
          if (i == 29 && j > 3)
            continue;
          unsigned char mask = 1 << j;
          unsigned char aMasked = a._bytes[i] & mask;
          unsigned char bMasked = b._bytes[i] & mask;
          aMasked = aMasked >> j;
          bMasked = bMasked >> j;
          if (aMasked > bMasked)
          {
            return true;
          }
          else if (bMasked > aMasked)
          {
            return false;
          }
        }
      }
    }
    return true;
  }
  friend bool operator>(const bigFloat &d, const bigFloat &c)
  {
    bigFloat a = c;
    bigFloat b = d;
    //Get the signal
    {
      unsigned char mask = 1 << 7;
      unsigned char bmasked = b._bytes[31] & mask;
      bmasked = bmasked >> 7;
      unsigned char amasked = amasked & a._bytes[31];
      amasked = amasked >> 7;
      if (amasked != 0 && bmasked == 0)
        return false;
      else if (bmasked != 0 && amasked == 0)
        return false;
      else if (bmasked != 0 && amasked != 0)
      {
        a = c;
        b = d;
      }
    }
    //Get exponent of the a
    long int aExponent = 0;
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
        unsigned char bit = a._bytes[i] & mask;
        bit = bit >> j;
        aExponent += (bit * pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //- bias
    aExponent -= 262143;
    //Get this exponent
    long int bExponent = 0;
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
        unsigned char bit = b._bytes[i] & mask;
        bit = bit >> j;
        bExponent += (bit * bigFloat::pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //denormalize
    bExponent -= 262143;
    if (aExponent > bExponent)
    {
      return false;
    }
    else if (aExponent < bExponent)
    {
      return true;
    }
    else
    {
      for (char i = 29; i >= 0; i--)
      {
        for (char j = 7; j >= 0; j--)
        {
          if (i == 29 && j > 3)
            continue;
          unsigned char mask = 1 << j;
          unsigned char aMasked = a._bytes[i] & mask;
          unsigned char bMasked = b._bytes[i] & mask;
          aMasked = aMasked >> j;
          bMasked = bMasked >> j;
          if (aMasked > bMasked)
          {
            return false;
          }
          else if (bMasked > aMasked)
          {
            return true;
          }
        }
      }
    }
    return false;
  }
  friend bool operator>=(const bigFloat &d, const bigFloat &c)
  {
    bigFloat a = c;
    bigFloat b = d;
    //Get the signal
    {
      unsigned char mask = 1 << 7;
      unsigned char bmasked = b._bytes[31] & mask;
      unsigned char amasked = amasked & a._bytes[31];
      if (amasked != 0 && bmasked == 0)
        return false;
      else if (bmasked != 0 && amasked == 0)
        return false;
      else if (bmasked != 0 && amasked != 0)
      {
        a = c;
        b = d;
      }
    }
    //Get exponent of the a
    long int aExponent = 0;
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
        unsigned char bit = a._bytes[i] & mask;
        bit = bit >> j;
        aExponent += (bit * pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //- bias
    aExponent -= 262143;
    //Get this exponent
    long int bExponent = 0;
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
        unsigned char bit = b._bytes[i] & mask;
        bit = bit >> j;
        bExponent += (bit * bigFloat::pow((int)2, (unsigned int)eTwoCount));
        eTwoCount++;
      }
    }
    //denormalize
    bExponent -= 262143;
    if (aExponent > bExponent)
    {
      return false;
    }
    else if (aExponent < bExponent)
    {
      return true;
    }
    else
    {
      for (char i = 29; i >= 0; i--)
      {
        for (char j = 7; j >= 0; j--)
        {
          if (i == 29 && j > 3)
            continue;
          unsigned char mask = 1 << j;
          unsigned char aMasked = a._bytes[i] & mask;
          unsigned char bMasked = b._bytes[i] & mask;
          aMasked = aMasked >> j;
          bMasked = bMasked >> j;
          if (aMasked > bMasked)
          {
            return false;
          }
          else if (bMasked > aMasked)
          {
            return true;
          }
        }
      }
    }
    return true;
  }
  //Out value
  double out()
  {
    double o = 0;
    //Get exponent
    long int exponent = 0;
    int eTwoCount = 0;
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
    eTwoCount = -236;
    for (unsigned char i = 0; i < 30; i++)
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
    o = (1 + mantisa) * pow((double)2, (int)exponent);
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
  //for ints
  static long long int pow(int n, unsigned int e)
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
  static double pow(double n, int e)
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
  //rigth shift
  void RightShiftMantisa(unsigned char *mantisa, unsigned int a)
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
        int minId = j - a;
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