//#include <iostream>
//#include <vector>
//#include <string.h>
//#include <string>
//#include "AES.h"
//using namespace std;
//
//class EncryptAES: public AES
//{
//private:
//    struct subTable
//    {
//        const string input[16] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };
//        const string output[16] = { "1010", "0000", "1001", "1110", "0110", "0011", "1111", "0101", "0001", "1101", "1100", "0111", "1011", "0100", "0010", "1000" };
//    } table;
//
//public:
//
//    EncryptAES():AES() {
//        //empty
//    }
//
//    void SubAllNibbles()
//    {
//        for (int i = 0; i < 4; i++)
//        {
//            int index = 0;
//            bool found = false;
//
//            while ((index < 16) && (found == false))
//            {
//                if (table.input[index].compare(nibbles[i]) == 0)
//                {
//                    found = true;
//                    nibbles[i] = table.output[index]; //swapping
//                }
//                index++;
//            }
//        }
//    }
//
//    //finds the substitute of a particular nibble
//    string findSubOfNibble(string nibble) {
//        int index = 0;
//        bool found = false;
//
//        while ((index < 16) && (found == false))
//        {
//            if (table.input[index].compare(nibble) == 0)
//            {
//                found = true;
//                return table.output[index];
//            }
//            index++;
//        }
//
//        return "";
//    }
//
//    void MixColumnsEncrypt()
//    {
//        int i, j;
//
//        //first get denary values of each column
//        int columnValues[4];
//
//        for (j = 0; j < 4; j++)
//        {
//            columnValues[j] = binaryToInt(nibbles[j]);
//        }
//
//        int results[4];
//
//        results[0] = GF(1, columnValues[0]) ^ GF(4, columnValues[1]);
//        results[1] = GF(4, columnValues[0]) ^ GF(1, columnValues[1]);
//        results[2] = GF(1, columnValues[2]) ^ GF(4, columnValues[3]);
//        results[3] = GF(4, columnValues[2]) ^ GF(1, columnValues[3]);
//
//
//        for (i = 0; i < 4; i++)
//        {
//            nibbles[i] = intToBinary(results[i]);
//        }
//    }
//
//    void GenerateRoundKeys()
//    {
//        //generating first key
//        rKeyNibs1[0] = intToBinary(binaryToInt(keyNibbles[0]) ^ binaryToInt(findSubOfNibble(keyNibbles[3])) ^ 14);
//        rKeyNibs1[1] = intToBinary(binaryToInt(keyNibbles[1]) ^ binaryToInt(rKeyNibs1[0]));
//        rKeyNibs1[2] = intToBinary(binaryToInt(keyNibbles[2]) ^ binaryToInt(rKeyNibs1[1]));
//        rKeyNibs1[3] = intToBinary(binaryToInt(keyNibbles[3]) ^ binaryToInt(rKeyNibs1[2]));
//
//        //generating second key
//        rKeyNibs2[0] = intToBinary(binaryToInt(rKeyNibs1[0]) ^ binaryToInt(findSubOfNibble(rKeyNibs1[3])) ^ 10);
//        rKeyNibs2[1] = intToBinary(binaryToInt(rKeyNibs1[1]) ^ binaryToInt(rKeyNibs2[0]));
//        rKeyNibs2[2] = intToBinary(binaryToInt(rKeyNibs1[2]) ^ binaryToInt(rKeyNibs2[1]));
//        rKeyNibs2[3] = intToBinary(binaryToInt(rKeyNibs1[3]) ^ binaryToInt(rKeyNibs2[2]));
//    }
//
//    int AES_Steps_Demo()
//    {
//        cout << "Enter 4 hexadecimal values (16 bits): " << endl;
//        getline(cin, input);
//
//        if (input.length() != 4)
//        {
//            cout << "Input Length mismatch!" << endl;
//            return -1;
//        }
//
//        if (checkHex(input) == false)
//        {
//            cout << "Input not in hexadecimals!" << endl;
//            return -2;
//        }
//
//        for (int i = 0; i < 4; i++)
//        {
//            nibbles[i] = hexToBinary(input[i]);
//        }
//
//        //now we have a whole block of 4 nibbles in the form of an array of strings
//        //now we perform encryption
//
//        SubAllNibbles();
//        cout << "Applying Sub Nibbles: " << getResult() << endl;
//
//        resetNibbles();
//
//        /* AddRoundKey(nibbles, binaryKey);
//         cout << "Applying Add Round Key: " << endl;
//         displayResults(nibbles);*/
//
//        ShiftRow();
//        cout << "Applying Shift Row: " << getResult() << endl;
//
//        resetNibbles();
//
//        MixColumnsEncrypt();
//        cout << "Applying Mix Cols: " << getResult() << endl;
//
//        resetNibbles();
//
//        int flag = inputKey();
//        if (flag < 0)
//        {
//            return -1;
//        }
//
//        for (int i = 0; i < 4; i++)
//        {
//            keyNibbles[i] = hexToBinary(key[i]);
//        }
//
//        GenerateRoundKeys();
//
//        displayRoundKeys();
//    }
//};
//
//int main()
//{
//    EncryptAES aes;
//    aes.AES_Steps_Demo();
//
//    return 0;
//}