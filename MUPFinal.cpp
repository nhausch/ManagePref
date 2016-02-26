/*
 CIS 22B Manage User Preference
 File name: MUPFinal
 Purpose: Manage user preferences using an external file. Allow user login and change their preference. Also save a timestamp after every login.
 
 @author Nicholas Hausch
 @version 1.0
 @since   7/17/2015
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

// Constants

const int MAX_LEN = 20;

// Structure declaration

struct User
{
    int idNum;
    char usrName[MAX_LEN];
    char usrPassword[MAX_LEN];
    unsigned int timestamp;
    int gcPref;
    int isAdmin;
};

// Function prototypes

int commandLineArgs(int argc, char * argv[], char fileName[MAX_LEN]);
int manageUserPreference(int status, vector<User> UserArray, char fileName[MAX_LEN]);
int copyDataFile(vector<User> &UserArray, char fileName[MAX_LEN]);
int printArray(vector<User> &UserArray, int numUsers);
int regularLogin(vector<User> &UserArray, int numUsers);
int adminLogin(vector<User> &UserArray, int numUsers);
int sortArray(vector<User> &UserArray, int numUsers, int howSort);
int saveArray(vector<User> &UserArray, char fileName[MAX_LEN], int numUsers);
void makeLower(char (&word)[MAX_LEN]);
void displayPref(int pref);

// Main Function
int main(int argc, char * argv[])
{
    vector<User> UserArray;
    char fileName[MAX_LEN];
    int status = 0;
    status = commandLineArgs(argc,argv,fileName);
    // cout << "Status: " << status << endl;
    manageUserPreference(status,UserArray,fileName);
    return 0;
}

// Process command line args
int commandLineArgs(int argc, char * argv[], char fileName[MAX_LEN])
{
    strcpy(fileName,argv[2]);
    if (argc == 3 && strncmp(argv[1],"-datafile",20) == 0) {
        return 1;
    }
    else if (argc == 4 && (strncmp(argv[1],"-datafile",20) == 0) && (strncmp(argv[3],"-admin",20) == 0)) {
        return 2;
    }
    else {
        return 0;
    }
}

// Manage user preferences
int manageUserPreference(int status, vector<User> UserArray, char fileName[MAX_LEN])
{
    if (status == 0)
        return 0;
    int numUsers = 0;
    copyDataFile(UserArray,fileName);
    numUsers = UserArray.size();
    
    // printArray(UserArray,numUsers);
    
    if (status == 1) {
        regularLogin(UserArray,numUsers);
        saveArray(UserArray,fileName,numUsers);
    }
    
    if (status == 2)
        adminLogin(UserArray,numUsers);

    return 0;
}

// If the user is attempting to login as an admin
int adminLogin(vector<User> &UserArray, int numUsers) {
    
    char enteredID[MAX_LEN];
    char enteredPass[MAX_LEN];
    
    cout << endl << "Please enter User ID: ";
    cin >> enteredID;
    cout << "Please enter password: ";
    cin >> enteredPass;
    
    char enteredIDcopy[MAX_LEN];
    strcpy(enteredIDcopy,enteredID);
    makeLower(enteredIDcopy);
    
    int k = 0;
    int userMatch = 0;
    int passMatch = 0;
    char tempID[MAX_LEN];
    
    for (k = 0; k < numUsers; k++) {
        strcpy(tempID,UserArray[k].usrName);
        makeLower(tempID);
        if (strncmp(enteredIDcopy,tempID,20) == 0) {
            userMatch = 1;
            break;
        }
    }
    
    if (userMatch == 0) {
        cout << "User not found." << endl;
        return 0;
    }
    
    if (strncmp(UserArray[k].usrPassword,enteredPass,20) == 0) {
        passMatch = 1;
    }
    
    if (passMatch == 0) {
        cout << "Password is incorrect." << endl;
        return 0;
    }
    
    if (UserArray[k].isAdmin == 0) {
        cout << "You are not authorized to access the reports." << endl;
        return 0;
    }
    
    // Get the user's sorted order choice
    int sortChoice = 0;
    
    while (sortChoice != 4) {
        cout << endl << "Please select the sorted order for the report:" << endl << "    1. By User ID" << endl << "    2. By Last Login Time" << endl << "    3. Game Console Preference" << endl << "    4. Exit" << endl << "Enter your choice: ";
        cin >> sortChoice;
    
        if (sortChoice < 1 || sortChoice > 4) {
            cout << "Invalid choice. Program ended." << endl;
            return 0;
        }
    
        if (sortChoice == 4) {
            cout << "Program ended." << endl;
            break;
            return 0;
        }
        sortArray(UserArray,numUsers,sortChoice);
    }
    
    return 0;
}

// Classes to support sort function
struct myCompareClass1 {
    bool operator()(User elem1, User elem2) {
        
        return (elem1.idNum < elem2.idNum);
    };
} idCompareObj;

struct myCompareClass2 {
    bool operator()(User elem1, User elem2) {
        
        return (elem1.timestamp < elem2.timestamp);
    };
} timeStampCompareObj;

struct myCompareClass3 {
bool operator()(User elem1, User elem2) {
    
    return (elem1.gcPref < elem2.gcPref);
    };
} preferenceCompareObj;

// Sort according to the user's choice
int sortArray (vector<User> &UserArray, int numUsers, int howSort) {
    
    if (howSort == 1) {
        sort(UserArray.begin(), UserArray.end(), idCompareObj);
        printArray(UserArray,numUsers);
    }
    else if (howSort == 2) {
        sort(UserArray.begin(), UserArray.end(), timeStampCompareObj);
        printArray(UserArray,numUsers);
    }
    else {
        sort(UserArray.begin(), UserArray.end(), preferenceCompareObj);
        printArray(UserArray,numUsers);
    }
    
    return 0;
}

// Copy data file into main struct
int copyDataFile(vector<User> &UserArray, char fileName[MAX_LEN])
{
    // cout << fileName << endl;
    ifstream dataFile;
    dataFile.open(fileName, ios::in);
    
    int row = 0;
    int column = 0;
    int index = 0;
    int j = 0;
    char myCharacter;
    char myString[MAX_LEN];

    User oneUser ;
    UserArray.push_back(oneUser);
    
    while (!dataFile.eof()) {
        
        dataFile.get(myCharacter);
        
        if (myCharacter == '\n' || myCharacter == ' ') {
            if (column == 0){
                UserArray[row].idNum = atoi(myString);
                column++;
            }
            else if (column == 1) {
                strcpy(UserArray[row].usrName,myString);
                column++;
            }
            else if (column == 2) {
                strcpy(UserArray[row].usrPassword,myString);
                column++;
            }
            else if (column == 3) {
                UserArray[row].timestamp = atoi(myString);
                column++;
            }
            else if (column == 4) {
                UserArray[row].gcPref = atoi(myString);
                column++;
            }
            else if (column == 5) {
                UserArray[row].isAdmin = atoi(myString);
                column = 0;
                row++;
                UserArray.push_back(oneUser);
            }
            else {
                exit(EXIT_FAILURE);
            }
        index = 0;
        memset(myString,0,20);
        }
        else {
            myString[index] = myCharacter;
            index++;
        }
        UserArray[row].isAdmin = myCharacter - 48;
    }
    dataFile.close();
    // cout << "J:" << j << endl;
    return 0;
}

// Print entire struct
int printArray(vector<User> &UserArray, int numUsers)
{
    cout << endl;
    int i = 0;
    for (i = 0; i < numUsers; i++) {
        cout << UserArray[i].idNum << " ";
        cout << UserArray[i].usrName << " ";
        cout << UserArray[i].usrPassword << " ";
        cout << UserArray[i].timestamp << " ";
        cout << UserArray[i].gcPref << " ";
        cout << UserArray[i].isAdmin << endl;
    }
    
    return 0;
}

// Non-admin login
int regularLogin(vector<User> &UserArray, int numUsers)
{
    char enteredID[MAX_LEN];
    char enteredPass[MAX_LEN];
    
    cout << endl << "Please enter User ID: ";
    cin >> enteredID;
    cout << "Please enter password: ";
    cin >> enteredPass;
    
    char enteredIDcopy[MAX_LEN];
    strcpy(enteredIDcopy,enteredID);
    makeLower(enteredIDcopy);
    
    int k = 0;
    int userMatch = 0;
    int passMatch = 0;
    char tempID[MAX_LEN];
    
    // Check if the user exists
    for (k = 0; k < numUsers; k++) {
        strcpy(tempID,UserArray[k].usrName);
        makeLower(tempID);
        if (strncmp(enteredIDcopy,tempID,20) == 0) {
            userMatch = 1;
            break;
        }
    }
    
    if (userMatch == 0) {
        cout << "User not found." << endl;
        return 0;
    }
    
    if (strncmp(UserArray[k].usrPassword,enteredPass,20) == 0) {
        passMatch = 1;
    }
    
    if (passMatch == 0) {
        cout << "Password is incorrect." << endl;
        return 0;
    }
    
    displayPref(UserArray[k].gcPref);
    
    int newPref = 0;
    char inputChar;
    
    cout << "Please enter your new preference:" << endl << "    0 - No preference" << endl << "    1 - Wii" << endl << "    2 - Xbox" << endl << "    3 - Playstation" << endl << "    4 - Other" << endl << "Your choice (hit <return> to cancel): ";
    cin.ignore();
    inputChar = getchar();
    
    if (inputChar == '\n') {
        cout << "Program ended." << endl;
        return 0;
    }
    else {
        newPref = inputChar - 48;
    }
    
    if (newPref < 0 || newPref > 4) {
        cout << "Invalid entry. Program ended." << endl;
        return 0;
    }
    
    UserArray[k].gcPref = newPref;
    
    // Save timestamp
    time_t seconds;
    seconds = time(NULL);
    UserArray[k].timestamp = seconds;
    
    return 0;
}

// Save modified array with changes
int saveArray(vector<User> &UserArray, char fileName[MAX_LEN], int numUsers)
{
    fstream dataFile;
    int z = 0;
    
    dataFile.open(fileName, ios::out | ios::trunc);
    
    for (z = 0; z < numUsers; z++) {
        
        dataFile << UserArray[z].idNum << " ";
        dataFile << UserArray[z].usrName << " ";
        dataFile << UserArray[z].usrPassword << " ";
        dataFile << UserArray[z].timestamp << " ";
        dataFile << UserArray[z].gcPref << " ";
        dataFile << UserArray[z].isAdmin;
        if (z < (numUsers - 1)) {
            dataFile << endl;
        }
    }
    dataFile.close();
    cout << "Thank you. Your preference is saved." << endl;
    
    return 0;
}

// Convert char array to lowercase
void makeLower(char (&word)[MAX_LEN])
{
    int length = 0;
    length = strlen(word);
    
    int l = 0;
    
    for (l = 0; l < length; l++) {
        word[l] = tolower(word[l]);
    }
}

// Display current preference
void displayPref (int pref)
{
    cout << endl << "Your current preference is ";
    
    switch (pref)
    {
        case 0: cout << "no preference." << endl; break;
        case 1: cout << "Wii." << endl; break;
        case 2: cout << "Xbox." << endl; break;
        case 3: cout << "Playstation." << endl; break;
        case 4: cout << "other." << endl; break;
    }
}












