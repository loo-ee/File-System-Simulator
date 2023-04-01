#include <iostream>
#include "Tree.h"

using namespace std;

char getAction();
void createFolder(FileSystem<string> *);

int main() {
    FileSystem<string> *systemDirectory = new FileSystem<string>();

    char action;
    bool running = true;

    while (running) {
        cout << "<------- FILE SYSTEM SIMULATOR ------->\n";
        action = getAction();

        system("cls");

        switch (action) {
            case '1':
                systemDirectory->print(1);
                break;

            case '2':
                systemDirectory->print(0);
                break;

            case '3':
                createFolder(systemDirectory);
                break;

            case '8':
                delete[] systemDirectory;
                systemDirectory = new FileSystem<string>();
                break;
            
            case '9':
                running = false;
                break;
        }

        cout << endl;
    }
     

    return 0;
}

char getAction() {
    char choice;

    cout << "\n[ACTIONS]\n";
    cout << "[1] Show System Tree\n[2] Show System Tree-CMD mode\n"
        << "[3] Add Folder\n[4] Add File\n"
        << "[5] Search File/Folder\n[6] Delete File/Folder\n"
        << "[7] Edit File/Folder name\n[8] Delete all Files and Folders\n" 
        << "[9] Exit\n"
        << "Choose action: ";
    cin >> choice;

    return choice;
}

void createFolder(FileSystem<string> *systemDirectory) {
    const string VALUE = "";
    string folderName;
    string path;

    cout << "\nEnter folder name: ";
    getline(cin >> ws, folderName); 
    folderName += "/";

    cout << "Enter path to put folder into: ";
    getline(cin >> ws, path);

    if (systemDirectory->verifyPath(path + folderName)) {   
        cout << "\n[INFO] Folder exists! Aborting...\n";
        return;
    }

    systemDirectory->add(VALUE, "dir", folderName, path);
}