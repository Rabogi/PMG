#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>

using namespace std;

/**
 * @brief Class to hold user data.
 *
 */
class note
{
    string userPassword;
    string userComment;
    string category;

public:
    string site;
    string userName;
    note(string nSite, string nUserName, string nUserPass, string nCategory, string nUserComm)
    {
        this->site = nSite;
        if (nSite == "non" || nSite == "Undefined")
            this->site = "Undefined";

        this->userName = nUserName;
        if (nUserName == "non" || nUserName == "Undefined")
            this->userName = "Undefined";

        this->userPassword = nUserPass;
        if (nUserPass == "non" || nUserPass == "Undefined")
            this->userPassword = "Undefined";
            
        this->userComment = nUserComm;
        if (nUserComm == "non" || nUserComm == "Undefined")
            this->userComment = "Undefined";
            
        this->category = nCategory;
        if (nCategory == "non" || nCategory == "Undefined")
            this->category = "Undefined";
    }
    string getSite()
    {
        return site;
    }
    string getName()
    {
        return userName;
    }
    string getPass()
    {
        return userPassword;
    }
    string getComm()
    {
        return userComment;
    }
    string getCtgr()
    {
        return category;
    }
    void setSite(string input)
    {
        this->site = input;
    }
    void setName(string input)
    {
        this->userName = input;
    }
    void setPass(string input)
    {
        this->userPassword = input;
    }
    void setComm(string input)
    {
        this->userComment = input;
    }
    void setCategory(string input)
    {
        this->category = input;
    }

    string getNote(){
        return (site + " " + userName + " " + userPassword + " " + category + " " + userComment);
    }
};