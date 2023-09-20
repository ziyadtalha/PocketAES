#include <iostream>
#include <vector>
#include <string.h>
#include <string>
using namespace std;

class AES
{
private:
    //maps Denary, Binary, and HEX onto each other
    struct Conversion
    {
        string binary[16] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };
        int integers[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
        string hexadecimals = "0123456789abcdef";
    } const converter;

protected:

    string input;
    string nibbles[4];

    string key;
    string keyNibbles[4];

    string rKeyNibs1[4];
    string rKeyNibs2[4];

public:

    AES() {
        input = "";
        key = "";
        for (int i = 0; i < 4; i++)
        {
            nibbles[i] = "";
            keyNibbles[i] = "";
            rKeyNibs1[i] = "";
            rKeyNibs2[i] = "";
        }
    }

    bool checkHex(string str)
    {
        string hexadecimals = "0123456789abcdef";
        for (int i = 0; i < 4; i++)
        {
            //if character is found to be a hexadecimal
            if (converter.hexadecimals.find(str[i]) != string::npos)
            {
                //do nothing
            }
            //if a single character is not hex then we return false
            else
            {
                return false;
            }
        }
        return true;
    }

    char binaryToHex(string nibble)
    {
        for (int i = 0; i < 16; i++)
        {
            if (nibble.compare(converter.binary[i]) == 0)
            {
                return converter.hexadecimals[i];
            }
        }
        return '\0';
    }

    string hexToBinary(char character)
    {
        for (int i = 0; i < 16; i++)
        {
            if (character == char(converter.hexadecimals[i]))
            {
                return converter.binary[i];
            }
        }
        return "";
    }

    int binaryToInt(string nibble)
    {
        for (int i = 0; i < 16; i++)
        {
            if (nibble.compare(converter.binary[i]) == 0)
            {
                return converter.integers[i];
            }
        }
        return 0;
    }

    string intToBinary(int value)
    {
        if ((value < 16) && (value > 0))
        {
            return converter.binary[value];
        }
        return "0000";
    }

    void deepCopyNibbles(string newNibbles[4])
    {
        for (int i = 0; i < 4; i++)
        {
            newNibbles[i] += nibbles[i];
        }
    }

    void AddRoundKey1()
    {
        for (int i = 0; i < 4; i++)
        {
            int res = (binaryToInt(nibbles[i]) ^ binaryToInt(rKeyNibs1[i]));
            nibbles[i] = intToBinary(res);
        }
    }

    void AddRoundKey2() {
        for (int i = 0; i < 4; i++)
        {
            int res = (binaryToInt(nibbles[i]) ^ binaryToInt(rKeyNibs2[i]));
            nibbles[i] = intToBinary(res);
        }
    }

    int GF(int a, int b)
    {
        int m = 0;
        while (b > 0)
        {
            if ((b % 2) == 1)
            {
                m = (m ^ a);
            }
            a = (a << 1);
            if (a >= 16)
            {
                a = a ^ 19;
            }
            b = (b >> 1);
        }
        return m;
    }

    void ShiftRow()
    {
        string temp = nibbles[0];
        nibbles[0] = nibbles[2];
        nibbles[2] = temp;
    }

    string getResult()
    {
        string result = "";
        for (int i = 0; i < 4; i++)
        {
            result += binaryToHex(nibbles[i]);
        }
        return result;
    }

    void displayRoundKeys()
    {
        string rKey1;
        for (int i = 0; i < 4; i++)
        {
            rKey1 += binaryToHex(rKeyNibs1[i]);
        }
        cout << "Round Key 1: " << rKey1 << endl;

        string rKey2;
        for (int i = 0; i < 4; i++)
        {
            rKey2 += binaryToHex(rKeyNibs2[i]);
        }
        cout << "Round Key 2: " << rKey2 << endl;
    }

    void resetNibbles()
    {
        for (int i = 0; i < 4; i++)
        {
            nibbles[i] = hexToBinary(input[i]);
        }
    }

    int inputKey()
    {
        cout << "Enter Key of 4 hex values: " << endl;
        getline(cin, key);

        if (key.length() > 4)
        {
            cout << "Key Length mismatch!" << endl;
            return -1;
        }
        else
        {
            if (key.length() < 4)
            {
                cout << "Key not full! Fixing key!" << endl;
            }

            string temp;
            if (key.length() == 3)
            {

                temp = "0";
                temp += key;
                key = temp;
            }
            else
            {
                if (key.length() == 2)
                {
                    temp = "00";
                    temp += key;
                    key = temp;
                }
                else
                {
                    if (key.length() == 1)
                    {
                        temp = "000";
                        temp += key;
                        key = temp;
                    }
                }
            }
        }

        if (checkHex(key) == false)
        {
            cout << "Key not in hexadecimals!" << endl;
            return -2;
        }

        for (int i = 0; i < 4; i++)
        {
            keyNibbles[i] = hexToBinary(key[i]);
        }
    }
};
