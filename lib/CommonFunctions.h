#include <iostream>
#include "dirent.h"
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <vector>

vector<string> listSuitableFiles();
int findSubStr(string input, string sub);
void printNote(note ntp, int id);
int getPassRating(string pass);
string generatePass(string symbols, int length);

/**
 * @brief Method to create a list of .txt files in same dir as executable
 *
 * @return vector<string> list of txt files
 */
vector<string> listSuitableFiles()
{
    DIR *dr;
    struct dirent *en;
    vector<string> filenames;
    int t;
    dr = opendir("."); // Opening current dir
    if (dr)
    {
        while ((en = readdir(dr)) != NULL)
        {
            string tempA = en->d_name;
            // string.find() return first occurrence of substring given in arg
            t = tempA.find(".txt");
            // if t not equal default starting position of string means substring was found
            if (t != string::npos)
                filenames.push_back(tempA);
        }
        closedir(dr); // Closing dir
    }
    return filenames;
}

/**
 * @brief Method to find if input sting contains substring and get it position in input string
 *
 * @param input input string
 * @param sub search target
 * @return int position of substring start
 */
int findSubStr(string input, string sub)
{
    int t = input.find(sub);
    if (t != string::npos)
    {
        return t;
    }
    return -1;
}

/**
 * @brief Method to print notes data
 *
 * @param ntp note to be printed
 * @param id note id in vector
 */
void printNote(note ntp, int id)
{
    cout << "Note #" << id + 1 << endl;
    cout << "Category   : " << ntp.getCtgr() << endl;
    cout << "Commentary : " << ntp.getComm() << endl;
    cout << "Site       : " << ntp.getSite() << endl;
    cout << "Login data : " << ntp.getName() << endl;
    cout << "Password   : " << ntp.getPass() << endl;
    cout << endl;
}

/**
 * @brief Method to evaluate security of password
 *
 * @param pass input password
 * @return int Rating of password security
 */
int getPassRating(string pass)
{
    // table of security criteria
    int isLongEnough = 0;
    int hasLower = 0;
    int hasUpper = 0;
    int hasDigits = 0;
    int hasSpecial = 0;
    int passLen = pass.length();
    if (passLen > 0)
    {
        if (passLen > 8)
            isLongEnough = 1;
        for (int i = 0; i < passLen; i++)
        {
            if (islower(pass[i]))
                hasLower = 1;
            if (isupper(pass[i]))
                hasUpper = 1;
            if (isdigit(pass[i]))
                hasDigits = 1;
            if (!isdigit(pass[i]) && !isalpha(pass[i]))
                hasSpecial = 1;
        }
        return (hasLower + hasUpper + hasDigits + hasSpecial + isLongEnough);
    }
    else
        return -1;
}

/**
 * @brief Method to generate pseudo random password
 *
 * @param symbols dictionary of chars to be used in generation
 * @param length password length
 * @return string generated password
 */
string generatePass(string symbols, int length)
{
    string pass;
    if (length > 0)
    {
        for (int i = 0; i < length; i++)
        {
            pass.append(symbols, rand() % (symbols.size() + 1 - 0) - 0, 1); // appending pseudo random selected char to the end of string
        }
        return pass;
    }
    else
        return "error";
}