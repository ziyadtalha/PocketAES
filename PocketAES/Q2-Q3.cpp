#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <algorithm>
#include "AES.h"
using namespace std;

class DecryptAES : public AES
{
private:
    struct invertedSubTable
    {
        string output[16] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };
        string input[16] = { "1010", "0000", "1001", "1110", "0110", "0011", "1111", "0101", "0001", "1101", "1100", "0111", "1011", "0100", "0010", "1000" };
    } invertedTable;

    struct subTable
    {
        const string input[16] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };
        const string output[16] = { "1010", "0000", "1001", "1110", "0110", "0011", "1111", "0101", "0001", "1101", "1100", "0111", "1011", "0100", "0010", "1000" };
    } table;

    string plainText;

public:

    DecryptAES() :AES() {
        plainText = "";
    }

    void SubNibblesDecrypt()
    {
        for (int i = 0; i < 4; i++)
        {
            int index = 0;
            bool found = false;

            while ((index < 16) && (found == false))
            {
                if (invertedTable.input[index].compare(nibbles[i]) == 0)
                {
                    found = true;
                    nibbles[i] = invertedTable.output[index]; //swapping
                }
                index++;
            }
        }
    }

    //finds the substitute of a particular nibble from NON-INVERTED table
    string findSubOfNibble(string nibble) {
        int index = 0;
        bool found = false;

        while ((index < 16) && (found == false))
        {
            if (table.input[index].compare(nibble) == 0)
            {
                found = true;
                return table.output[index];
            }
            index++;
        }

        return "";
    }

    void MixColumnsDecrypt()
    {
        int i, j;

        //first get denary values of each column
        int columnValues[4];

        for (j = 0; j < 4; j++)
        {
            columnValues[j] = binaryToInt(nibbles[j]);
        }

        int results[4];

        results[0] = GF(9, columnValues[0]) ^ GF(2, columnValues[1]);
        results[1] = GF(2, columnValues[0]) ^ GF(9, columnValues[1]);
        results[2] = GF(9, columnValues[2]) ^ GF(2, columnValues[3]);
        results[3] = GF(2, columnValues[2]) ^ GF(9, columnValues[3]);


        for (i = 0; i < 4; i++)
        {
            nibbles[i] = intToBinary(results[i]);
        }
    }

    void GenerateRoundKeys()
    {
        //generating first key
        rKeyNibs1[0] = intToBinary(binaryToInt(keyNibbles[0]) ^ binaryToInt(findSubOfNibble(keyNibbles[3])) ^ 14);
        rKeyNibs1[1] = intToBinary(binaryToInt(keyNibbles[1]) ^ binaryToInt(rKeyNibs1[0]));
        rKeyNibs1[2] = intToBinary(binaryToInt(keyNibbles[2]) ^ binaryToInt(rKeyNibs1[1]));
        rKeyNibs1[3] = intToBinary(binaryToInt(keyNibbles[3]) ^ binaryToInt(rKeyNibs1[2]));

        //generating second key
        rKeyNibs2[0] = intToBinary(binaryToInt(rKeyNibs1[0]) ^ binaryToInt(findSubOfNibble(rKeyNibs1[3])) ^ 10);
        rKeyNibs2[1] = intToBinary(binaryToInt(rKeyNibs1[1]) ^ binaryToInt(rKeyNibs2[0]));
        rKeyNibs2[2] = intToBinary(binaryToInt(rKeyNibs1[2]) ^ binaryToInt(rKeyNibs2[1]));
        rKeyNibs2[3] = intToBinary(binaryToInt(rKeyNibs1[3]) ^ binaryToInt(rKeyNibs2[2]));
    }

    void decryptBlock() {
        GenerateRoundKeys();

        ShiftRow();

        AddRoundKey2();

        SubNibblesDecrypt();

        ShiftRow();

        MixColumnsDecrypt();

        AddRoundKey1();

        SubNibblesDecrypt();

        plainText += hexToAscii(getResult());
    }

    int decryptDemo()
    {
        cout << "Enter ciphertext in hexadecimal (16 bits): " << endl;
        getline(cin, input);

        if (input.length() != 4)
        {
            cout << "Input Length mismatch!" << endl;
            return -1;
        }

        if (checkHex(input) == false)
        {
            cout << "Input not in hexadecimals!" << endl;
            return -2;
        }

        for (int i = 0; i < 4; i++)
        {
            nibbles[i] = hexToBinary(input[i]);
        }

        inputKey();

        decryptBlock();

        cout << "Decrypted Cipher Text is: " << getResult() << endl;
    }

    bool checkInputFileFormat(string &rawText, int &count)
    {
        string temp = "";

        //removing null padding
        for (int i = 0; i < rawText.length(); i++)
        {
            if (rawText[i] != ' ')
            {
                temp += rawText[i];
            }
        }

        rawText = temp;

        while ((rawText.length() % 4) != 0)
        {
            //append "null" hex to the end
            rawText += '0';
            count++;
        }

        //check if every character in input file is in HEX format or not
        return checkHex(rawText);
    }

    int decryptFile() {
        string rawText = "";
        string temp = "";

        ifstream MyReadFile("secret.txt");
        while (getline(MyReadFile, temp)) {
            rawText += temp;
        }
        MyReadFile.close();

        cout << rawText << endl;

        //to remember how many "null" we appended to the rawText (cannot be > 3)
        int appendCount = 0;
        if (checkInputFileFormat(rawText, appendCount) == false)
        {
            cout<<"Input file format error!"<<endl;
            return -1;
        }

        inputKey();

        int blocks = (rawText.length() / 4);
        for (int i = 0; i < blocks; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                int index = (i * 4) + j;
                string nibble = hexToBinary(rawText[index]);
                nibbles[j] = nibble;
            }

            decryptBlock();

            //removing "null" characters
            if (appendCount > 0)
            {
                plainText = plainText.substr(0, (plainText.length() - appendCount));
            }
        }

        cout << "Decrypted Cipher Text is: " << plainText << endl;

        //writing this plaintext to a file
        ofstream MyFile("plain.txt");
        MyFile << plainText;
        MyFile.close();

        return 0;
    }
};

int main()
{
    DecryptAES aes;

    aes.decryptFile();

    aes.decryptDemo();

    return 0;
}