#include <dirent.h>
#include "lib/FileCrypt.h"
#include "lib/Storage.h"
#include "lib/CommonFunctions.h"
#include "ctime"

int main()
{
    // use cls if on windows or clear if on unix or linux systems
    // string consoleClear = "cls";
    string consoleClear = "clear";
    encMaster master;
    Storage coll;
    string menu;
    string tempDecrypted = "tempD", inputFile = " ";
    vector<string> txts;
    string site, name, pass, ctgr, comm, line;
    fstream decTemp;
    int lineCount = 0, menuInt = -1;
    bool readyToWork = false, keySet = false, wasDecrypted = false, newFile = false;
    system(consoleClear.c_str());

    while (true) // loop for initial setup on launch
    {
        if (readyToWork == false)
        {
            cout << endl
                 << "Choose option: " << endl;
            cout << "1)Set encryption/decryption key" << endl;
            cout << "2)Set input file" << endl;
            cout << "3)Decryption attempt and preview of contents" << endl;
            cout << "4)Proceed to work with set file " << endl;
            cout << "5)Create a new file" << endl;
            cout << endl
                 << "0)Exit" << endl;
            cout << endl
                 << "Your choice : ";
            cin >> menu;
            system(consoleClear.c_str());
            try // stoi(std::string) throws exception if input string cannot be properly converted to int
            {
                menuInt = stoi(menu); // converting menu option input from string to int
            }
            catch (...)
            {
                menuInt = -1; // setting menu variable to -1 will cause menu to print error message and continue working
            }
            switch (menuInt)
            {
            case 1: // input for encryption/decryption key
            {
                cin.clear();
                fflush(stdin);
                cout << "Input encryption/decryption key : ";
                string key = "-1";
                cin >> key;
                master.inputKey(key, 16);
                keySet = true;
                wasDecrypted = false;
                break;
            }
            case 2: // choosing input file
            {
                string choice = "-1";
                int choiceInt;
                txts = listSuitableFiles();
                cout << "Select your working file :" << endl;
                for (int i = 0; i < txts.size(); i++)
                {
                    cout << i + 1 << ")" << txts[i] << endl;
                }
                cout << "Your choice: ";
                cin >> choice;
                try // stoi(std::string) throws exception if input string cannot be properly converted to int
                {
                    choiceInt = stoi(choice); // converting menu option input from string to int
                }
                catch (...)
                {
                    choiceInt = -1; // setting menu variable to -1 will cause menu to print error message and continue working
                }
                if (choiceInt >= 1 && choiceInt <= txts.size())
                {
                    inputFile = txts[choiceInt - 1];
                    cout << "Selected file : " << inputFile << endl;
                    wasDecrypted = false;
                    newFile = false;
                    break;
                }
                else
                {
                    wasDecrypted = false;
                    newFile = false;
                    cout << "Invalid input. No file was selected." << endl;
                }
            }
            case 3: // try to decrypt a file and preview data
            {
                if (newFile == false)
                {
                    if (inputFile == " ")
                    {
                        cout << "Input file is not set!" << endl;
                        break;
                    }
                    if (keySet == false)
                    {
                        cout << "Encryption/decryption key is not set!" << endl;
                        break;
                    }
                    try
                    {
                        master.decrypt(inputFile, tempDecrypted);
                        decTemp.open(tempDecrypted, fstream::in);
                        lineCount = 0;
                        while (!decTemp.eof())
                        {
                            getline(decTemp, line);
                            lineCount++;
                        }
                        decTemp.close();
                        if (lineCount != 0)
                        {
                            if (coll.Load(tempDecrypted))
                            {
                                if (coll.StorageVec.size() == 1)
                                    cout << "Data in file : " << coll.StorageVec[0].getNote() << endl;
                                if (coll.StorageVec.size() > 1)
                                {
                                    cout << "Data in file : " << coll.StorageVec[0].getNote() << endl;
                                    cout << "and more..." << endl;
                                }
                            }
                        }
                        else
                        {
                            cout << "Decrypted data appears to be empty!" << endl;
                        }
                        wasDecrypted = true;
                    }
                    catch (...)
                    {
                        cout << "Decryption seems to have failed!" << endl;
                        fstream ftemp;
                        ftemp.open(tempDecrypted, fstream::in);
                        getline(ftemp, line);
                        cout << "First line of decryption result :" << line << endl;
                    }
                }
                if (newFile == true)
                {
                    cout << "This is new file! No decryption is possible with it!" << endl;
                }
                break;
            }
            case 4:
            {
                if (inputFile != " ")
                {
                    if (keySet == true)
                    {
                        if (wasDecrypted == true && newFile == false)
                        {
                            cout << "Are you sure you want to work with " << inputFile << " ?" << endl;
                            cout << "Type 'yes' to continue : ";
                            string answer;
                            cin >> answer;
                            if (answer == "Y" || answer == "Yes" || answer == "y" || answer == "yes")
                            {
                                readyToWork = true;
                                cout << "Proceeding to work with " << inputFile << "." << endl;
                                coll.Load(tempDecrypted);
                                coll.getDict();
                            }
                            else
                                cout << "No positive answer received!" << endl;
                        }
                        else
                            cout << "File was not decrypted and loaded!" << endl;
                        if (newFile == true)
                        {
                            cout << "Are you sure you want to work with " << inputFile << " ?" << endl;
                            cout << "Type 'yes' to continue : ";
                            string answer;
                            cin >> answer;
                            if (answer == "Y" || answer == "Yes" || answer == "y" || answer == "yes")
                            {
                                readyToWork = true;
                                cout << "Proceeding to work with " << inputFile << "." << endl;
                            }
                            else
                                cout << "No positive answer received!" << endl;
                        }
                    }
                    else
                    {
                        cout << "Encryption/decryption key is not set!" << endl;
                    }
                }
                else
                {
                    cout << "Input file is not set!" << endl;
                }
                break;
            }
            case 5:
            {
                newFile = true;
                cout << "Type a new file name : ";
                cin >> inputFile;
                if (inputFile == " " || findSubStr(inputFile, ".txt") < 1)
                {
                    cout << "Invalid name!" << endl;
                    inputFile = " ";
                }
                else
                {
                    ofstream newFile(inputFile);
                    newFile.open(inputFile, ios::in);
                    newFile.close();
                }
                break;
            }
            case 0:
            {
                return 0;
                break;
            }
            default:
            {
                cout << "Invalid input!" << endl;
                break;
            }
            }
        }
        else
            break;
    }

    coll.addCtgr("Undefined");
    while (true) // loop for working with file
    {
        cout << endl
             << "Choose option: " << endl;
        cout << "1)List all loaded notes" << endl;
        cout << "2)Search notes" << endl;
        cout << "3)Add note" << endl;
        cout << "4)Remove note" << endl;
        cout << "5)Edit note" << endl;
        cout << "6)Add category" << endl;
        cout << "7)Remove category and corresponding notes" << endl;
        cout << "8)List categories" << endl;
        cout << "9)Sort (by user name)" << endl;
        cout << "10)Sort (by site)" << endl;
        cout << endl
             << "0)Exit" << endl;
        cout << endl
             << "Your choice : ";
        cin >> menu;
        system(consoleClear.c_str());
        try // stoi(std::string) throws exception if input string cannot be properly converted to int
        {
            menuInt = stoi(menu); // converting menu option input from string to int
        }
        catch (...)
        {
            menuInt = -1; // setting menu variable to -1 will cause menu to print error message and continue working
        }
        switch (menuInt)
        {
        case 0:
        {
            return 0;
            break;
        }
        // List all loaded notes
        case 1:
        {
            master.decrypt(inputFile, tempDecrypted);
            coll.Load(tempDecrypted);
            if (coll.StorageVec.size() > 0)
            {
                cout << "Notes loaded : " << coll.StorageVec.size() << endl
                     << endl;
                for (int i = 0; i < coll.StorageVec.size(); i++)
                {
                    printNote(coll.StorageVec[i], i);
                }
            }
            else
            {
                cout << "There are no notes available!" << endl;
            }
            break;
        }
        case 2: // Search for passwords
        {
            master.decrypt(inputFile, tempDecrypted);
            coll.Load(tempDecrypted);
            cout << "Notes loaded : " << coll.StorageVec.size() << endl
                 << endl;
            if (coll.StorageVec.size() > 0)
            {
                string query;
                cout << "Enter your query (Case sensetive): ";
                cin.ignore(1000, '\n');
                cin >> query;
                coll.searchNotes(query);
                if (coll.CurrentVec.size() > 0)
                {
                    for (int i = 0; i < coll.CurrentVec.size(); i++)
                    {
                        printNote(coll.CurrentVec[i], i);
                    }
                }
                else
                {
                    cout << "No notes containing your query were found!" << endl;
                }
            }
            else
            {
                cout << "There are no notes available!" << endl;
            }
            break;
        }
        // Adding new note
        case 3:
        {
            string tempSite, tempName, tempComm, tempCtgr, tempPass;
            system(consoleClear.c_str());
            master.decrypt(inputFile, tempDecrypted);
            coll.Load(tempDecrypted);
            coll.getDict();
            cout << "Choose category of new note (Choose Undefined to exclude it from categorized search): " << endl;
            coll.listDict();
            cout << endl
                 << "You choice : ";
            string ctgrChoice = "0";
            int ctgrChoiceInt = 0;
            cin >> ctgrChoice;
            try // stoi(std::string) throws exception if input string cannot be properly converted to int
            {
                ctgrChoiceInt = stoi(ctgrChoice); // converting menu option input from string to int
                if (ctgrChoiceInt >= 0 && ctgrChoiceInt < coll.ctgrDict.size())
                {
                    tempCtgr = coll.ctgrDict[ctgrChoiceInt];
                    cout << "Choosen category : " << tempCtgr << " !" << endl;
                }
                else
                {
                    tempCtgr = coll.ctgrDict[0];
                    cout << "Invalid input.Defaulting to undefined!" << endl;
                }
            }
            catch (...)
            {
                cout << "Invalid input.Defaulting to undefined!" << endl;
                tempCtgr = "Undefined";
            }
            cout << "Input site data (type non for undefined): ";
            cin >> tempSite;
            cout << "Input username (type non for undefined): ";
            cin >> tempName;
            cout << "Input comment with '_' instead of spaces (type non for undefined): ";
            cin >> tempComm;

            cin.ignore(1000, '\n');
            bool passSet = false;
            int passChoiceInt = 0;
            while (true)
            {
                cout << "Do you want to set your own password (input 1) or to generate a new one(input 2)?" << endl
                     << "Choice : ";
                string passChoice;
                cin >> passChoice;
                system(consoleClear.c_str());
                try // stoi(std::string) throws exception if input string cannot be properly converted to int
                {
                    passChoiceInt = stoi(passChoice); // converting menu option input from string to int
                    break;
                }
                catch (...)
                {
                    cout << "Invalid input!";
                    passChoiceInt = -1;
                }
            }
            switch (passChoiceInt)
            {
            case 1:
            {
                cout << "Input your password : ";
                cin >> tempPass;
                int secR = getPassRating(tempPass);
                cout << "Security rating of this password is " << secR << "/5" << endl;
                passSet = true;
                break;
            }
            case 2:
            {
                string symbols = "";
                string passChoice;
                while (true)
                {
                    symbols = "";
                    cout << "Do you want your password to possibly have lower case symbols ? " << endl
                         << "Type '1' for yes, anything else for 'no' :";
                    cin >> passChoice;
                    if (passChoice == "1")
                    {
                        symbols = symbols + "abcdefghijklmnopqrstuvwxyz";
                    }
                    cout << "Do you want your password to possibly have UPPER case symbols ? " << endl
                         << "Type '1' for yes, anything else for 'no' :";
                    cin >> passChoice;
                    if (passChoice == "1")
                    {
                        symbols = symbols + "ABCDEFGHIJKLMNOQPRSTUYWVZX";
                    }

                    cout << "Do you want your password to possibly have digits? " << endl
                         << "Type '1' for yes, anything else for 'no' :";
                    cin >> passChoice;
                    if (passChoice == "1")
                    {
                        symbols = symbols + "0123456789";
                    }

                    cout << "Do you want your password to possibly have special symbols? " << endl
                         << "Type '1' for yes, anything else for 'no' :";
                    cin >> passChoice;
                    if (passChoice == "1")
                    {
                        symbols = symbols + "!@#$^&*?";
                    }
                    if (symbols.size() > 1)
                        break;
                    else
                        system(consoleClear.c_str());
                    cout << "No symbol array were selected to be used in generation!" << endl;
                }

                while (true)
                {
                    system(consoleClear.c_str());
                    tempPass = generatePass(symbols, 20);
                    int tempR = getPassRating(tempPass);
                    cout << "Your password : " << tempPass << endl;
                    cout << "Security rating of this password is " << tempR << "/5" << endl;
                    cout << "Do you want to use it or to generate a new one? Input 1 to generate a new password or anything else to apply : ";
                    cin >> tempR;
                    if (tempR != 1)
                    {
                        passSet = true;
                        break;
                    }
                }
                break;
            }
            default:
            {
                break;
            }
            }
            if (passSet == true)
            {
                coll.addNote(note(tempSite, tempName, tempPass, tempCtgr, tempComm));
                coll.writeData(tempDecrypted);
                master.encrypt(inputFile, tempDecrypted);
                coll.Load(tempDecrypted);
            }
            break;
        }
        // Removing note
        case 4:
        {
            system(consoleClear.c_str());
            master.decrypt(inputFile, tempDecrypted);
            coll.Load(tempDecrypted);
            if (coll.StorageVec.size() > 0)
            {
                cout << "Notes loaded : " << coll.StorageVec.size() << endl
                     << endl;
                for (int i = 0; i < coll.StorageVec.size(); i++)
                {
                    printNote(coll.StorageVec[i], i);
                }
                cout << "Input number of note that you want to remove  :";
                string remChoice;
                cin >> remChoice;
                int remChoiceInt = 0;
                try // stoi(std::string) throws exception if input string cannot be properly converted to int
                {
                    remChoiceInt = stoi(remChoice); // converting menu option input from string to int
                    remChoiceInt--;
                }
                catch (...)
                {
                    cout << "Invalid input! No note was removed" << endl;
                    break;
                }
                if (remChoiceInt >= 0 && remChoiceInt <= coll.StorageVec.size())
                {
                    system(consoleClear.c_str());
                    cout << "Are you sure you want to delete following note (Input 1 for 'yes' and anything else for 'no'): " << endl;
                    printNote(coll.StorageVec[remChoiceInt], true);
                    cin >> remChoice;
                    if (remChoice == "1")
                    {
                        system(consoleClear.c_str());
                        cout << "Following note was deleted !" << endl;
                        printNote(coll.StorageVec[remChoiceInt], true);
                        coll.remNote(remChoiceInt);
                        coll.writeData(tempDecrypted);
                        master.encrypt(inputFile, tempDecrypted);
                        coll.Load(tempDecrypted);
                    }
                    else
                    {
                        system(consoleClear.c_str());
                        cout << "Note was not deleted" << endl;
                    }
                }
                else
                {
                    system(consoleClear.c_str());
                    cout << "Invalid input! No note was removed" << endl;
                }
            }
            else
            {
                system(consoleClear.c_str());
                cout << "There are no notes available!" << endl;
            }
            break;
        }
        case 5:
        {
            system(consoleClear.c_str());
            master.decrypt(inputFile, tempDecrypted);
            coll.Load(tempDecrypted);
            cout << "Input id of note you want to edit :";
            string editCh;
            int editChInt = 0;
            cin.ignore(1000, '\n');
            cin >> editCh;
            try // stoi(std::string) throws exception if input string cannot be properly converted to int
            {
                editChInt = stoi(editCh); // converting menu option input from string to int
                editChInt--;
            }
            catch (...)
            {
                cout << "Invalid input! No note was modified" << endl;
                break;
            }
            coll.editNote(editChInt);
            coll.writeData(tempDecrypted);
            master.encrypt(inputFile, tempDecrypted);
            coll.Load(tempDecrypted);
            break;
        }
        // removing a category and every note with it
        case 6:
        {
            system(consoleClear.c_str());
            master.decrypt(inputFile, tempDecrypted);
            coll.Load(tempDecrypted);
            coll.getDict();
            string tempCtgr;
            cout << "Input new category : ";
            cin >> tempCtgr;
            coll.addCtgr(tempCtgr);
            break;
        }
        // Removing a category
        case 7:
        {
            system(consoleClear.c_str());
            master.decrypt(inputFile, tempDecrypted);
            coll.Load(tempDecrypted);
            coll.getDict();
            coll.listDict();
            cout << "Select category to remove : ";
            int ctr;
            cin >> ctr;
            coll.remCtgr(ctr);
            coll.writeData(tempDecrypted);
            master.encrypt(inputFile, tempDecrypted);
            coll.Load(tempDecrypted);
            break;
        }
        // List all categories
        case 8:
        {
            system(consoleClear.c_str());
            master.decrypt(inputFile, tempDecrypted);
            coll.Load(tempDecrypted);
            coll.getDict();
            coll.listDict();
            break;
        }
        case 9:{
            system(consoleClear.c_str());
            master.decrypt(inputFile, tempDecrypted);
            coll.Load(tempDecrypted);
            coll.sortStorage(1);
            break;
        }
        case 10:{
            system(consoleClear.c_str());
            master.decrypt(inputFile, tempDecrypted);
            coll.Load(tempDecrypted);
            coll.sortStorage(2);
            break;
        }

        break;
        }
    }
}
