#include "teste.hpp"

class bigFloat
{
  public:
    unsigned char _byte[512];
    //Construtor
    bigFloat()
    {
        for (unsigned char i = 0; i < 128; i++)
            _byte[i] = 0;
    }
    //Construtor int
    bigFloat(int value)
    {
        for (int i = 0; i < 512; i++)
            _byte[i] = 0;
        int count = 0;
        for (char i = 0; i < 4; i++) //Para cada byte do int
        {
            for (char j = 0; j < 8; j++) //Para cada bit do byte
            {
                int mask = 1 << count;
                mask = mask >> i * 8;
                value = value >> i * 8;
                int masked_n = value & mask;
                _byte[i] = _byte[i] | masked_n;
                count++;
            }
        }
    }
    bigFloat(const char *value)
    {
        for (int i = 0; i < 512; i++)
            _byte[i] = 0;
        unsigned long long int count = 0;
        unsigned long long int strLen = strlen(value) - 1;
        while (count <= strLen)
        {
            if (value[strLen - count] == '.')
            {
                count++;
                continue;
            }
            else if (value[strLen - count] == '-')
            {
                count++;
                continue;
            }
            int c;
            if (value[strLen - count] == '0')
                c = 0;
            else if (value[strLen - count] == '1')
                c = 1;
            else if (value[strLen - count] == '2')
                c = 2;
            else if (value[strLen - count] == '3')
                c = 3;
            else if (value[strLen - count] == '4')
                c = 4;
            else if (value[strLen - count] == '5')
                c = 5;
            else if (value[strLen - count] == '6')
                c = 6;
            else if (value[strLen - count] == '7')
                c = 7;
            else if (value[strLen - count] == '8')
                c = 8;
            else if (value[strLen - count] == '9')
                c = 9;
            int uc = 0;
            for (unsigned char i = 0; i < 7; i++)
            {
                int mask = 1 << i;
                int nMask = c & mask;
                uc = uc | nMask;
            }

            if (count > 0)
            {
                //Fator
                bigFloat factor = 1;
                for (unsigned long long int i = 0; i < count; i++)
                {
                    factor = factor * 10;
                }
                factor = factor * uc;
                bigFloat f = *this + factor;
                for (int i = 0; i < 512; i++)
                {
                    this->_byte[i] = f._byte[i];
                }
            }
            else
            {
                ByteSum(uc);
            }
            count++;
        }
    }
    bigFloat(bigFloat *a)
    {
        for (int i = 0; i < 512; i++)
        {
            this->_byte[i] = a->_byte[i];
        }
        this->_negative = a->_negative;
    }
    bigFloat operator*(const bigFloat &a) const
    {
        bigFloat b = 0;
        bigFloat c = 0;
        int leftZero = 0;
        for (int i = 0; i < 512; i++)
        {
            for (char j = 0; j < 8; j++)
            {
                unsigned char mask = 1 << j;
                unsigned char masked = mask & this->_byte[i];
                unsigned char bit = masked >> j;
                if (bit == 1)
                {
                    if (i == 0 && j == 0)
                    {
                        b = a;
                        leftZero++;
                    }
                    else
                    {
                        c = a << leftZero;
                        b = b + c;
                        leftZero++;
                    }
                }
                else
                {
                    c = 0;
                    leftZero++;
                }
            }
        }
        std::cout << std::bitset<8>(b._byte[2]) << std::bitset<8>(b._byte[1]) << std::bitset<8>(b._byte[0]) << std::endl
                  << std::endl;
        return b;
    }
    bigFloat operator*(const int &a) const
    {
        bigFloat numInt = a;
        bigFloat b = *this * numInt;
        return b;
    }
    bigFloat operator-(const bigFloat &a)
    {
        bigFloat b = ~a;
        b = b + 1;
        b = *this + b;
        return b;
    }
    bigFloat operator~() const
    {
        bigFloat a = 0;
        for (int i = 0; i < 512; i++)
        {
            a._byte[i] = ~this->_byte[i];
        }
        return a;
    }
    bigFloat operator+(const int &a)
    {
        bigFloat b = a;
        b = b + this;
        return b;
    }
    bigFloat operator+(const bigFloat &a)
    {
        unsigned char carry = 0;
        unsigned char bit = 0;
        //unsigned int bitCount = 0;
        bigFloat b = this;
        for (int i = 0; i < 512; i++)
        {
            for (char j = 0; j < 8; j++)
            {
                unsigned char mask = 1 << j; //mascara
                unsigned char aMasked = a._byte[i] & mask;
                unsigned char tMasked = this->_byte[i] & mask;
                unsigned char aBit = aMasked >> j;
                unsigned char tBit = tMasked >> j;
                bit = ((aBit ^ tBit) ^ carry);
                carry = ((aBit & tBit) | (aBit & carry)) | (tBit & carry);
                if (bit == 1)
                {
                    int bMask = 1 << j;
                    b._byte[i] = b._byte[i] | bMask;
                }
                else
                {
                    int bMask = 1 << j;
                    bMask = ~bMask;
                    b._byte[i] = b._byte[i] & bMask;
                }
            }
        }
        return b;
    }
    bigFloat operator=(const int &a)
    {
        {
            int value = a;
            int count = 0;
            for (char i = 0; i < 4; i++) //Para cada byte do int
            {
                for (char j = 0; j < 8; j++) //Para cada bit do byte
                {
                    int mask = 1 << count;
                    mask = mask >> i * 8;
                    value = value >> i * 8;
                    int masked_n = value & mask;
                    this->_byte[i] = this->_byte[i] | masked_n;
                    count++;
                }
            }
        }
    }
    bigFloat operator=(const bigFloat &a)
    {
        bigFloat b = a;
        for (int i = 0; i < 512; i++)
        {
            this->_byte[i] = b._byte[i];
        }
        this->_negative = b._negative;
    }
    bigFloat operator<<(const int &a) const
    {
        bigFloat b = *this;
        bigFloat c = 0;
        for (int i = 0; i < 512; i++)
        {
            for (char j = 0; j < 8; j++)
            {
                unsigned char mask = 1 << j;
                unsigned char masked = mask & b._byte[i];
                unsigned char bit = masked >> j;
                if ((a + j) < 8)
                {
                    masked = masked << a;
                    c._byte[i] = masked | c._byte[i];
                    std::cout << std::bitset<8>(c._byte[2]) << std::bitset<8>(c._byte[1]) << std::bitset<8>(c._byte[0]) << "\n";
                }
                else if (bit == 1)
                {
                    char bitId = (a + j) % 8;
                    int byteId = ((a + (j + (i * 8))) - bitId) / 8;
                    if (byteId < 512)
                    {
                        masked = 1 << bitId;
                        c._byte[byteId] = masked | c._byte[byteId];
                        std::cout << std::bitset<8>(c._byte[2]) << std::bitset<8>(c._byte[1]) << std::bitset<8>(c._byte[0]) << "\n";
                    }
                }
            }
        }
        return c;
    }

  private:
    bool _negative = false;
    void Multiply(bigFloat a, bigFloat b)
    {
    }
    void ByteSum(int num)
    {
        int c = 0;
        int bit = 0;
        for (int i = 0; i < 8; i++)
        {
            int mask = 1 << i;
            int numMasked = num & mask;
            int bigMasked = _byte[0] & mask;
            int numBit = numMasked >> i;
            int bigBit = bigMasked >> i;

            bit = ((numBit ^ bigBit) ^ c); // c is carry
            c = ((numBit & bigBit) | (numBit & c)) | (bigBit & c);
            if (bit == 1)
            {
                int bMask = 1 << i;
                _byte[0] = _byte[0] | bMask;
            }
            else
            {
                int bMask = 1 << i;
                bMask = ~bMask;
                _byte[0] = _byte[0] & bMask;
            }
        }
        int byteCount = 1;
        int bitCount = 0;
        for (int i = 8; i < 511 * 8 && c == 1; i++) //Cada bit
        {
            if (bitCount > 7)
            {
                byteCount++;
                bitCount = 0;
            }
            int mask = 1 << bitCount;
            int bigMasked = _byte[byteCount] & mask;
            int bigBit = bigMasked >> bitCount;

            bit = (bigBit ^ c); // c is carry
            c = (bigBit & c);
            if (bit == 1)
            {
                int bMask = 1 << bitCount;
                _byte[byteCount] = _byte[byteCount] | bMask;
            }
            else
            {
                int bMask = 1 << bitCount;
                bMask = ~bMask;
                _byte[byteCount] = _byte[byteCount] & bMask;
            }
            bitCount++;
        }

        std::cout << std::bitset<8>(_byte[7]) << std::bitset<8>(_byte[6]) << std::bitset<8>(_byte[5]) << std::bitset<8>(_byte[4]) << std::bitset<8>(_byte[3]) << std::bitset<8>(_byte[2]) << std::bitset<8>(_byte[1]) << std::bitset<8>(_byte[0]) << std::endl
                  << std::endl;
    }
};

int main()
{
    bigFloat m = "10";
    bigFloat n = "5";
    m = n - m;
    bigFloat d = (char *)10;
    std::cout << "f";
    return 0;
}
