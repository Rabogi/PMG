#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>

using namespace std;

/**
 * @brief Class that contains encoding/decoding methods
 */
class encMaster
{
    string keyPhrase;
    int key = -1;
    char c;
    void del_line(string file_name, int n);
    void trimEnd(string file_name, int size, int cut);

public:
    int inputKey(string keyUncut, int length);
    string enc = "encrypted.txt";
    string dec = "decrypted.txt";
    void encrypt();
    void decrypt();
    void encrypt(string enc, string dec);
    void decrypt(string enc, string dec);
};
/**
 * @brief This method shifts every char variable (on ASCII table) in given file for a set offset. Offset is determined by key.
 *
 */
void encMaster::encrypt()
{
    fstream fin, fout;

    fin.open(dec, fstream::in);
    fout.open(enc, fstream::out);
    while (fin >> noskipws >> c) // Encryption of every char in file
    {
        int temp = (c + key);
        fout << (char)temp;
    }
    fin.close();
    fout.close();
    fout.open(enc, fstream::app);

    time_t now = time(0); // Addition of timestamp
    char *date_time = ctime(&now);
    fout << "\n";
    fout << string(date_time);
    fout.close();
}

/**
 * @brief This method shifts every char variable (on ASCII table) in given file for a set offset. Offset is determined by key.
 *
 * @param enc Encrypted output file
 * @param dec Decrypted input file
 */
void encMaster::encrypt(string enc, string dec)
{
    fstream fin, fout;

    fin.open(dec, fstream::in);
    fout.open(enc, fstream::out);
    while (fin >> noskipws >> c) // Encryption of every char in file
    {
        int temp = (c + key);
        fout << (char)temp;
    }
    fin.close();
    fout.close();
    fout.open(enc, fstream::app);

    time_t now = time(0); // Addition of timestamp
    char *date_time = ctime(&now);
    fout << "\n";
    fout << string(date_time);
    fout.close();
}

/**
 * @brief This method shifts every char variable (on ASCII table) in given file back for a set offset. Offset is determined by key.
 *
 */
void encMaster::decrypt()
{
    int count = 0;
    string line;
    fstream fin, fout, ftemp;

    fin.open(enc, fstream::in); // Counting amount of rows in text file
    while (!fin.eof())
    {
        getline(fin, line);
        count++;
    }
    fin.close();

    fin.open(enc, fstream::in); // Getting and printing time and date of last decryption
    string lastTimeDecrypted;
    for (int i = 1; i <= count - 1; i++)
        getline(fin, lastTimeDecrypted);
    cout << "Last time decrypted : " << lastTimeDecrypted << endl;
    fin.close();

    del_line(enc, count - 1); // removing timestamp

    fin.open(enc, fstream::in); // Reverting encryption
    fout.open(dec, fstream::out);
    while (fin >> noskipws >> c)
    {
        int temp = (c - key);
        fout << (char)temp;
    }
    fin.close();
    fout.close();
    // Because we have a special form for the encrypted data with the timestamp,
    // we have to account for stray newline that can end up in decrypted file.
    fout.open(dec, fstream::app);
    int size = fout.tellg();
    fout.close();

    trimEnd(dec, size, 1); // removing stray newLine produced by decryption

    fin.open(enc, fstream::app); // Addition of timestamp
    time_t now = time(0);
    char *date_time = ctime(&now);
    fin << string(date_time);
    fin.close();
}

/**
 * @brief This method shifts every char variable (on ASCII table) in given file back for a set offset. Offset is determined by key.
 *
 * @param enc Encrypted input file
 * @param dec Decrypted output file
 */
void encMaster::decrypt(string enc, string dec)
{
    int count = 0;
    string line;
    fstream fin, fout, ftemp;

    fin.open(enc, fstream::in); // Counting amount of rows in text file
    while (!fin.eof())
    {
        getline(fin, line);
        count++;
    }
    fin.close();

    fin.open(enc, fstream::in); // Getting and printing time and date of last decryption
    string lastTimeDecrypted;
    for (int i = 1; i <= count - 1; i++)
        getline(fin, lastTimeDecrypted);
    cout << "Last time decrypted : " << lastTimeDecrypted << endl;
    fin.close();

    del_line(enc, count - 1); // removing timestamp

    fin.open(enc, fstream::in); // Reverting encryption
    fout.open(dec, fstream::out);
    while (fin >> noskipws >> c)
    {
        int temp = (c - key);
        fout << (char)temp;
    }
    fin.close();
    fout.close();
    // Because we have a special form for the encrypted data with the timestamp,
    // we have to account for stray newline that can end up in decrypted file.
    fout.open(dec, fstream::app);
    int size = fout.tellg();
    fout.close();

    trimEnd(dec, size, 1); // removing stray newLine produced by decryption

    fin.open(enc, fstream::app); // Addition of timestamp
    time_t now = time(0);
    char *date_time = ctime(&now);
    fin << string(date_time);
    fin.close();
}

/**
 * @brief This method makes sure we get a valid key by cutting it and summing char values into one integer.
 *
 * @param messages Displayed message for input
 * @param length How many symbols will be used in key before discarding remaining part of input. This is done to avoid integer overflows.
 * @return int
 */
int encMaster::inputKey(string keyUncut, int length)
{
    string keyCut;
    keyCut = keyUncut.substr(0, length); // Discarding any symbols after set length (16 is best here because it will limit key and prevent stack underflows that may have been caused by bigger keys)

    // This block limits how long char array converted from string will be
    // This prevents repeated inputs of keys generating different results
    int size = 0;
    if (keyCut.length() <= length)
        size = keyCut.length();
    else
        size = length;

    char *chArr = new char[size];

    strcpy(chArr, keyCut.c_str()); // converting std::string into char array

    int keySum = 0;
    // Key is generated by sum of all of its chars ASCII values
    for (int i = 0; i < size; i++)
    {
        keySum += chArr[i];
    }
    this->key = keySum;
    return keySum;
}

/**
 * @brief Method to delete specified line in file
 *
 * @param file_name input file
 * @param n line number (starting from 0)
 */
void encMaster::del_line(string file_name, int n)
{
    ifstream fileToCut(file_name);
    ofstream f_temp;
    f_temp.open("temp.txt", ios::out);

    char ch;
    int line = 1;
    // Copying every char from original file to new one expect ones located on *deleted* line
    while (fileToCut.get(ch))
    {
        if (ch == '\n')
            line++;
        if (line != n)
            f_temp << ch;
    }

    f_temp.close();
    fileToCut.close();
    // replacing original file with new file
    remove(file_name.c_str());
    rename("temp.txt", file_name.c_str());
}

/**
 * @brief Method to remove stray newline symbols generated during decryption
 *
 * @param file_name input file
 * @param size end position of line
 * @param cut how many chars should be deleted
 */
void encMaster::trimEnd(string file_name, int size, int cut)
{
    ifstream fileToTrim(file_name);
    ofstream f_temp;
    f_temp.open("temp.txt", ios::out);

    char ch;
    int count = size - cut;
    int i = 0;
    while (fileToTrim.get(ch) && i < count)
    {
        f_temp << ch;
        i++;
    }

    f_temp.close();
    fileToTrim.close();
    remove(file_name.c_str());
    rename("temp.txt", file_name.c_str());
}