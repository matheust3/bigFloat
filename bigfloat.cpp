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

    if (d1.parts.sign == 1)
    {
      unsigned char mask = 1 << 7;
      this->_bytes[31] = this->_bytes[31] | mask;
    }

    for (unsigned char i = 0; i < 32; i++)
    {
      for (unsigned char j = 0; j < 8; j++)
      {
      }
    }
  }

private:
  unsigned char _bytes[32];
};
using namespace std;
int main()
{
  return 0;
}
