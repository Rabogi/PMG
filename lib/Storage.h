#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include "Note.h"

using namespace std;
/**
 * @brief Class that will load,hold and manipulate notes and their data
 *
 */
class Storage
{
private:
public:
    vector<string> ctgrDict;
    vector<note> StorageVec;
    vector<note> CurrentVec;
    bool isLoaded = false;
    bool Load(string decryptedFile);
    bool addNote(note nta);
    bool addNote(string nta);
    bool addNote(note nta, vector<note> target);
    bool remNote(int id);
    bool writeData(string decryptedFile);
    bool printNote(int id, vector<note> vec, bool printId);
    void printNoteCard(note ntp, int id);
    bool searchNotes(string query);
    void getDict();
    void listDict();
    void addCtgr(string ctgr);
    void remCtgr(int id);
    void recDel(vector<note> vec, vector<note> toBeDeleted);
    bool areEqual(note a, note b);
    void editNote(int id);
    void sortStorage(int sortTerm);
    vector<int> loadCategory(string Ctgr);
    Storage();
};

Storage::Storage()
{
}

/**
 * @brief Method to parse decrypted file and add all Notes stored in it. Notes are added to StorageVec.
 *
 * @param decryptedFile Input file
 */
bool Storage::Load(string decryptedFile)
{
    StorageVec.clear();
    string line;
    fstream fin;
    fin.open(decryptedFile, fstream::app);
    int size = fin.tellg();
    fin.close();
    if (size != 0)
    {
        fin.open(decryptedFile, fstream::in);
        while (!fin.eof())
        {
            getline(fin, line);
            if (line != "")
            {
                addNote(line);
                // cout << line << endl;
            }
        }
        fin.close();
    }
    // Removing decrypted file is vital to preserve security of data
    if (StorageVec.size() > 0)
    {
        isLoaded = true;
        remove(decryptedFile.c_str());
        ofstream newFile(decryptedFile);
        newFile.open(decryptedFile, ios::in);
        newFile.close();
        return true;
    }
    else
        return false;
}

/**
 * @brief Method to add Note to temporal storage in StorageVec
 *
 * @param nta Note object to be added to StorageVec
 */
bool Storage::addNote(note nta)
{
    this->StorageVec.push_back(nta);
    return true;
}

/**
 * @brief Method to add Note to temporal storage in tarter vector
 *
 * @param nta Note object to be added to target vector
 * @param target vector where new note will be added
 */
bool Storage::addNote(note nta, vector<note> target)
{
    target.push_back(nta);
    return true;
}

/**
 * @brief Method to add Note to temporal storage in StorageVec
 *
 * @param nta string from which new Note object will be generated
 */
bool Storage::addNote(string nta)
{
    string del = " ";
    vector<string> tokens;
    int pos = 0;
    while ((pos = nta.find(del)) != string::npos)
    {
        tokens.push_back(nta.substr(0, pos));
        nta.erase(0, pos + 1);
    }
    tokens.push_back(nta);
    note temp = note(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]);
    this->StorageVec.push_back(temp);
    return true;
}

/**
 * @brief Method that writes all Notes stored in StorageVec
 *
 * @param decryptedFile temporal storage file
 */
bool Storage::writeData(string decryptedFile)
{
    int size = this->StorageVec.size();
    ofstream fout;
    string stringToW;
    fout.open(decryptedFile, ios::out | ios::trunc);
    for (int i = 0; i < size; i++)
    {
        stringToW = "";
        stringToW += this->StorageVec[i].getSite() + " ";
        stringToW += this->StorageVec[i].getName() + " ";
        stringToW += this->StorageVec[i].getPass() + " ";
        stringToW += this->StorageVec[i].getCtgr() + " ";
        stringToW += this->StorageVec[i].getComm();
        fout << stringToW << endl;
    }
    fout.close();
    return true;
}

/**
 * @brief Method to print Note obj's data from specified vector
 *
 * @param id array pos of Note
 * @param vec input vector
 * @param printId Should Note's id on current vector be shown or not
 * @return true id is in bounds of vector
 * @return false id isn't in bounds of vector
 */
bool Storage::printNote(int id, vector<note> vec, bool printId)
{
    int size = StorageVec.size();
    if (id >= 0 && id < size)
    {
        if (printId == true)
            cout << id << ") ";
        cout << vec[id].getNote() << endl;
        return true;
    }
    else
        return false;
}

/**
 * @brief Method to print note data as card
 *
 * @param ntp note to be printed
 * @param id index in array
 */
void Storage::printNoteCard(note ntp, int id)
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
 * @brief Method to find all notes from StorageVec containing query data in note's name,password,site or commentary. All found notes will be pushed in CurrentVec.
 *
 * @param query search data
 * @return true if found any
 * @return false if haven't found any
 */
bool Storage::searchNotes(string query)
{
    CurrentVec.clear();
    bool found = false;
    int fPoint;
    for (int i = 0; i < StorageVec.size(); i++)
    {
        found = false;
        fPoint = StorageVec[i].getName().find(query);
        if (fPoint != string::npos)
            found = true;
        fPoint = StorageVec[i].getPass().find(query);
        if (fPoint != string::npos)
            found = true;
        fPoint = StorageVec[i].getComm().find(query);
        if (fPoint != string::npos)
            found = true;
        fPoint = StorageVec[i].getSite().find(query);
        if (fPoint != string::npos)
            found = true;
        fPoint = StorageVec[i].getCtgr().find(query);
        if (fPoint != string::npos)
            found = true;
        if (found == true)
        {
            CurrentVec.push_back(StorageVec[i]);
        }
    }
    if (CurrentVec.size() > 0)
        return true;
    else
        return false;
}

/**
 * @brief Method that gets all unique categories from stored notes
 *
 */
void Storage::getDict()
{
    for (int i = 0; i < StorageVec.size(); i++)
    {
        addCtgr(StorageVec[i].getCtgr());
    }
}

/**
 * @brief Method to list all stored categories
 *
 */
void Storage::listDict()
{
    for (int i = 0; i < ctgrDict.size(); i++)
    {
        cout << i << ")" << ctgrDict[i] << endl;
    }
}

/**
 * @brief Method to add new category
 *
 * @param ctgr category to be added
 */
void Storage::addCtgr(string ctgr)
{
    bool found = false;
    for (int i = 0; i < ctgrDict.size(); i++)
    {
        if (ctgrDict[i] == ctgr)
            found = true;
    }
    if (found == false)
        ctgrDict.push_back(ctgr);
}

/**
 * @brief Method to remove a category and every note with this category
 *
 * @param id id for category in ctgrDict
 */
void Storage::remCtgr(int id)
{
    string categoryName = ctgrDict[id];
    if (ctgrDict[id] != "Undefined")
    {
        vector<int> ids = loadCategory(ctgrDict[id]);
        for (int i = 0; i < ids.size(); i++)
        {
            printNoteCard(StorageVec[i], ids[i]);
        }
        cout << "Listed notes to be deleted! Input 'Delete!' to delete them :";
        string deleteCh;
        cin.ignore(1000, '\n');
        cin >> deleteCh;
        if (deleteCh == "Delete!")
        {
            recDel(StorageVec, CurrentVec);
            ctgrDict.erase(ctgrDict.begin() + id);
            getDict();
            cout << "Category '" << categoryName << "' and all of notes with this category were deleted!" << endl;
        }
        else
        {
            cout << "Nothing was deleted!" << endl;
        }
    }
    else
        cout << "This category cannot be removed" << endl;
}

/**
 * @brief Method to remove note
 *
 * @param id index in StorageVec
 * @return true Successful deletion
 * @return false error
 */
bool Storage::remNote(int id)
{
    if (id >= 0 && id <= StorageVec.size())
    {
        StorageVec.erase(StorageVec.begin() + id);
        return true;
    }
    return false;
}

/**
 * @brief Method to load every note of some category into Current vec
 *
 * @param Ctgr Category to search
 * @return vector<int> list of correct note ids
 */
vector<int> Storage::loadCategory(string Ctgr)
{
    CurrentVec.clear();
    vector<int> idList;
    for (int i = 0; i < StorageVec.size(); i++)
    {
        if (StorageVec[i].getCtgr() == Ctgr)
        {
            CurrentVec.push_back(StorageVec[i]);
            idList.push_back(i);
        }
    }
    return idList;
}

/**
 * @brief Method to delete all notes corresponding to set ids
 *
 * @param ids list of notes to be deleted
 * @param vec vector where to delete notes
 */
void Storage::recDel(vector<note> vec, vector<note> toBeDeleted)
{
    for (int i = 0; i < toBeDeleted.size(); i++)
    {
        for (int j = 0; j < vec.size(); j++)
        {
            if (areEqual(vec[j], toBeDeleted[i]))
                remNote(j);
        }
    }
}

/**
 * @brief Method to determine of 2 note are equal
 *
 * @return true are equal
 * @return false not equal
 */
bool Storage::areEqual(note a, note b)
{
    if (a.getComm() == b.getComm() && a.getCtgr() == b.getCtgr() && a.getSite() == b.getSite() && a.getName() == b.getName() && a.getPass() == b.getPass())
        return true;
    else
        return false;
}

/**
 * @brief Method that allow to edit existing note
 *
 * @param id id of note
 */
void Storage::editNote(int id)
{
    if (id >= 0 && id <= StorageVec.size())
    {
        cout << "Do you want to change this note (Input 'Yes!' to edit this note)?" << endl;
        printNoteCard(StorageVec[id], id);
        string editCh, dontCh = "!dont!";
        cin >> editCh;
        if (editCh == "Yes!")
        {
            string tempName, tempPass, tempCtgr, tempComm, tempSite;
            int tempCtgrInt = 0;
            cout << "Input new user name : ";
            cin.ignore(1000, '\n');
            cin >> tempName;
            cout << "Input new password : ";
            cin.ignore(1000, '\n');
            cin >> tempPass;
            cout << "Input new site : ";
            cin.ignore(1000, '\n');
            cin >> tempSite;
            cout << "Input new commentary with '_' instead of spaces : ";
            cin.ignore(1000, '\n');
            cin >> tempComm;
            getDict();
            cout << "Choose category : " << endl;
            listDict();
            cout << "Your choice : ";
            cin.ignore(1000, '\n');
            cin >> tempCtgrInt;
            if (tempCtgrInt >= 0 && tempCtgrInt <= ctgrDict.size())
            {
                tempCtgr = ctgrDict[tempCtgrInt];
            }
            else
            {
                cout << "Invalid input! Defaulting to 'Undefined'!" << endl;
                tempCtgr = ctgrDict[0];
            }
            note tempNote = note(tempSite, tempName, tempPass, tempCtgr, tempComm);
            cout << "Do you wish to replace following note :" << endl;
            printNoteCard(StorageVec[id], id);
            cout << "...with" << endl;
            printNoteCard(tempNote, id);
            cout << "Type 'Yes!' to replace : ";
            cin.ignore(1000, '\n');
            cin >> editCh;
            if (editCh == "Yes!")
            {
                StorageVec[id] = tempNote;
                cout << "Changes applied!" << endl;
            }
            else
                cout << "Discarding changes!" << endl;
        }
        else
        {
            cout << "No note were modified!" << endl;
        }
    }
    else
        cout << "Invalid input! No note were modified!" << endl;
}

/**
 * @brief Method to sort vector by term
 *
 * @param sortTerm 1 by name 2 by site
 */
void Storage::sortStorage(int sortTerm)
{
    if (StorageVec.size() > 0)
    {
        CurrentVec.clear();
        for (int i = 0; i < StorageVec.size(); i++)
        {
            CurrentVec.push_back(StorageVec[i]);
        }

        switch (sortTerm)
        {

        case 1: // sort by name
        {
            sort(CurrentVec.begin(), CurrentVec.end(), [](const note &first, const note &sec) { // User name comparator
                if (first.userName < sec.userName)
                    return true;
                else
                    return false;
            });
            for (int i = 0; i < CurrentVec.size(); i++)
            {
                printNoteCard(CurrentVec[i], i);
            }
            break;
        }
        case 2: // sort by site
        {
            sort(CurrentVec.begin(), CurrentVec.end(), [](const note &first, const note &sec) { // Site address comparator
                if (first.site < sec.site)
                    return true;
                else
                    return false;
            });
            for (int i = 0; i < CurrentVec.size(); i++)
            {
                printNoteCard(CurrentVec[i], i);
            }
            break;
        }
        default:
        {
            cout << "No such sorting mode!" << endl;
            ;
            break;
        }
        }
    }
    else
    {
        cout << "Nothing to sort!" << endl;
    }
}
