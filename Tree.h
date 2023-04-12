#ifndef TREE_TREE_H
#define TREE_TREE_H

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <conio.h>

#include "globals.h"

using namespace std;

template<typename T>
class FileSystem {
    struct TreeNode {
        T data;
        string type;
        string nodeName;
        string path;
        TreeNode *parentNode;
        vector<TreeNode *> children;
    };

    TreeNode *root = nullptr;

    void addChild(TreeNode *parentNode, TreeNode *child, string path) {
        string currentFilePath = parentNode->path;

        if (parentNode->path == path) {
            if (locateNode(this->root, (path + child->nodeName), true)) {
                cout << "\n[INFO] " << child->type << ": " << child->nodeName << " in path \"" << path << "\" already exists.\n";
                return;
            }

            if (parentNode->type == "file") {
                cout << "Cannot append a file to another file!\n";
                return;
            } 

            child->parentNode = parentNode;
            child->path = path + child->nodeName;
            parentNode->children.push_back(child);
            cout << "Insertion of " << child->type << ": " << child->nodeName << " done.\n";
            return;
        }

        for (TreeNode *x: parentNode->children) {
            addChild(x, child, path);
        }
    }

    void print_CMD_mode(TreeNode *node) {
        cout << "\nName: " << node->nodeName << endl;
        cout << "Type: " << node->type << endl;
        cout << "Path: " << node->path << endl;
        
        if (node->type == "file") {
            cout << "Value: " << node->data << endl;
        }
        
        if (node->parentNode)
            cout << "Parent: " << node->parentNode->path << endl;

        for (TreeNode* child : node->children) {
            print_CMD_mode(child);
        }
    }

    void printFileTree(TreeNode *node, int tabCount, string selectedPath) {
        static TreeNode *previousNode = nullptr;
        static TreeNode *currentParent = nullptr;
        TreeNode *nodePtr = currentParent;
        int counter = 1;
        HANDLE chandle = GetStdHandle(STD_OUTPUT_HANDLE);

        if (node == this->root) {
            cout << "\33[2C" << node->nodeName << " (root)\n";
        } else {
            cout << "\r";

            while (nodePtr) {
                if (nodePtr == node->parentNode) {
                    tabCount--;
                    break;
                } 

                nodePtr = nodePtr->parentNode; 
            }

            for (int i = 0; i < tabCount; i++) {
                cout << "\33[8C";
            }
            
            cout << "\33[1B|\33[1D\33[1B|";
            cout << "--> ";

            if (node->path == selectedPath) {
                cout << setColor(GREEN, chandle);
            }

            cout << node->nodeName;

            if (node->path == selectedPath) {
                cout << setColor(RED, chandle) << " * ";
            }

            cout << setColor(WHITE, chandle);
            cout << "\n"; 
        }

        tabCount++;
        previousNode = node;

        if (node->children.size() == 0) 
            return; 

        if (node->type == "file")
            currentParent = node->parentNode;

        for (TreeNode* child : node->children) {
            printFileTree(child, tabCount, selectedPath);
        }
    }

    TreeNode *locateNode(TreeNode *node, string path, bool firstRun) {
        static TreeNode *foundNode = nullptr;

        if (firstRun)
            foundNode = nullptr;

        if (node->path == path)
            foundNode = node;

        for (TreeNode *x: node->children)
            locateNode(x, path, false);

        return foundNode;
    }

    void editFileName(string path, string newName) {
        TreeNode *searchFile = nullptr;
        TreeNode *foundNode = locateNode(this->root, path, true);

        if (!foundNode) {
            cout << "\n[INFO] File not found.\n";
            return;
        }

        string fileToSearch = foundNode->parentNode->path + newName;
        searchFile = locateNode(this->root, fileToSearch, true);

        if (searchFile) {
            cout << "\n[INFO] File with name:" << newName << " already exists in specified directory.\n";
            return;
        }

        foundNode->nodeName = newName;
        foundNode->path = foundNode->parentNode->path + newName;

        cout << "\n[INFO] File renamed\n";
    }

    void printPathChoices(TreeNode *nodePtr, int selectedPathChoice) {
        HANDLE chandle = GetStdHandle(STD_OUTPUT_HANDLE);

        cout << setColor(YELLOW, chandle);
        cout << "\nDirectory: ";
        cout << setColor(WHITE, chandle);
        cout << nodePtr->path << endl;
        cout << setColor(RED, chandle);
        cout << "\n[Select Directory or File]\n";
        cout << setColor(WHITE, chandle);

        if (selectedPathChoice == 0) {
            cout << setColor(RED, chandle);
            cout << "--> ";
            cout << setColor(GREEN, chandle);
            cout << "[0] ../ \n";
            cout << setColor(WHITE, chandle);
        }
        else 
            cout << "    [0] ../ \n";

        if (nodePtr->type == "dir") {
            if (nodePtr->children.size() == 0) {
                cout << setColor(RED, chandle);
                cout << "\n[INFO]";
                cout << setColor(WHITE, chandle);
                cout << " No more sub directories\n";
            }

            for (int i = 1; i <= nodePtr->children.size(); i++) {
                if (selectedPathChoice == i) {
                    cout << setColor(RED, chandle);
                    cout << "--> ";
                    cout << setColor(GREEN, chandle);
                    cout << "[" << i << "] " << nodePtr->children[i -1]->nodeName << endl;
                    cout << setColor(WHITE, chandle);
                }
                else {
                    cout << "    ";
                    cout << "[" << i << "] " << nodePtr->children[i -1]->nodeName << endl;
                }
            }
        }
        else {
            cout << "\nName: " << nodePtr->nodeName << endl;
            cout << "Path: " << nodePtr->path << endl;
            cout << "Value: " << nodePtr->data << endl;
            
            if (nodePtr->parentNode)
                cout << "Parent: " << nodePtr->parentNode->path << endl;

            cout << endl;
        }
        
        cout << "\n[PRESS [S] to select]\n";

        if (nodePtr->children.size() != 0) {
            cout << "[PRESS [D] to delete]\n";
            cout << "[PRESS [E] to edit]\n";
        }

        cout << "[PRESS [N] to create new folder]\n";
        cout << "[PRESS [F] to create new file]\n";
    }

    string getPath(TreeNode *node, int selectedIndex) {
        if (selectedIndex == 0 && node->parentNode) 
            return node->parentNode->path;

        else if (selectedIndex == 0 && !node->parentNode)
            return "";

        else 
            return node->children[selectedIndex -1]->path;
    }

    bool isChoiceInvalid(int key) {
        const int KEYS_COUNT = 7;
        int keys[KEYS_COUNT] = {
            KEY_UP,
            KEY_DOWN,
            KEY_SELECT,
            KEY_DELETE,
            KEY_EDIT,
            KEY_ADD_FOLDER,
            KEY_ADD_FILE
        };

        for (int i = 0; i < KEYS_COUNT; i++) {
            if (keys[i] == key)
                return false;
        }

        return true;
    }

    void verifyDeletion(vector<TreeNode *> *children, int index) {
        bool running = true;
        int choice = 0;

        cout << "\n[INFO] Directory is not empty.\n";
        cout << "Press [O] to override\n";
        cout << "Press [Q] to exit\n";

        while (running) {
            choice = 0;

            while (choice != KEY_OVERRIDE && choice != KEY_EXIT) {
                choice = _getch();
            }

            switch (choice) {
                case KEY_OVERRIDE:
                    deleteNode(children, index);
                    cout << "\n[INFO] Folder deleted.\n";
                    running = false;
                    break;

                case KEY_EXIT:
                    cout << "\n[INFO] Aborted.\n";
                    running = false;
                    break;
            } 
        }
    }

    void deleteNode(vector<TreeNode *> *children, int index) {
        TreeNode *temp = children->at(index);

        children->erase(children->begin() + index);
        delete temp;
    }

public:
    FileSystem() {
        this->root = new TreeNode();

        this->root->type = "dir";
        this->root->nodeName = "/";
        this->root->path = "/";
        this->root->parentNode= nullptr;
    };

    void add(T value, string type, string nodeName, string parentName) {
        TreeNode *newNode = new TreeNode();

        newNode->type = type;
        newNode->data = value;
        newNode->nodeName= nodeName; 

        addChild(this->root, newNode, parentName);   
    }

    void print(int mode, string selectedPath) {
        HANDLE chandle = GetStdHandle(STD_OUTPUT_HANDLE);
        cout << setColor(RED, chandle);

        switch (mode) {
            case 0:
                cout << "\n[PRINTING CMD MODE]\n\n";
                cout << setColor(WHITE, chandle);
                print_CMD_mode(this->root);
                break;

            case 1:
                cout << "\n[PRINTING TREE]\n\n";
                cout << setColor(WHITE, chandle);
                printFileTree(this->root, 0, selectedPath);
                break;
        }
    }

    void searchNode(string path) {
        TreeNode *foundNode = locateNode(this->root, path, true);

        if (!foundNode) {
            cout << "\nNOT FOUND\n";
            return;
        }

        cout << "\nFOUND\n";
        cout << "Name: " << foundNode->nodeName << endl;
        cout << "Path: " << foundNode->path << endl;
        cout << "Type: " << foundNode->type << endl;

        if (foundNode->type == "file") 
            cout << "Value: " << foundNode->data << endl;
    }

    void deleteFile(string path) {
        TreeNode *nodeToDelete = locateNode(this->root, path, true);

        if (!nodeToDelete) {
            cout << "\n[INFO] File/Dir does not exist.\n";
            return;
        }

        vector<TreeNode*> *children = &nodeToDelete->parentNode->children;
        

        for (int i = 0; i < children->size(); i++) {
            if (children->at(i)->path == path) {
                if (children->at(i)->type == "dir") {
                    if (children->at(i)->children.size() != 0) {
                        verifyDeletion(children, i);
                        break;
                    }
                    else {
                        deleteNode(children, i);
                        break;
                    }
                }
                else {
                    deleteNode(children, i);
                    break;
                }
            }
        }
    }

    void renameFile(string path, string newName) {
        editFileName(path, newName);
    }

    bool verifyPath(string path) {
        TreeNode *foundNode = locateNode(this->root, path, true);

        return foundNode != nullptr;
    }

    void traverseFileSystem() {
        bool isRunning = true;
        bool is_SelectingPath = true;
        string path = "/";
        string folderName = "";
        string fileName = "";
        string fileValue = "";

        while (isRunning) {
            int choice = 0;
            int selectedPathChoice = 0;
            TreeNode *nodePtr = locateNode(this->root, path, true);

            is_SelectingPath = true;

            while (is_SelectingPath) {
                system("cls");
                print(1, path);
                printPathChoices(nodePtr, selectedPathChoice);

                cout << "\33[?25l";
                choice = 0;

                while (isChoiceInvalid(choice)) {
                    choice = _getch();
                }

                switch (choice) {
                    case KEY_UP:
                        selectedPathChoice--;

                        if (selectedPathChoice < 0) 
                            selectedPathChoice = 0;
                        break;

                    case KEY_DOWN:
                        selectedPathChoice++;

                        if (selectedPathChoice > nodePtr->children.size())
                            selectedPathChoice = static_cast<int>(nodePtr->children.size());
                        break;

                    case KEY_SELECT:
                        cout << "\33[?25h";
                        is_SelectingPath = false;

                        if (path == "/" && selectedPathChoice == 0)
                            isRunning = false;

                        path = getPath(nodePtr, selectedPathChoice);
                        break;

                    case KEY_DELETE:
                        if (selectedPathChoice == 0) 
                            break;

                        path = getPath(nodePtr, selectedPathChoice);        

                        if (path != "" && selectedPathChoice != 0) {
                            if (!verifyPath(path))
                                cout << "\n[WARNING] Invalid choice\n";
                            else 
                                deleteFile(path);

                            path = nodePtr->path;
                        }
                        break;

                    case KEY_ADD_FOLDER:
                        cout << "\nEnter folder name: ";
                        getline(cin >> ws, folderName);
                        folderName += "/";

                        if (verifyPath(path + folderName)) {   
                            cout << "\n[INFO] Folder exists! Aborting...\n";
                            return;
                        }

                        add("", "dir", folderName, path);
                        break;

                    case KEY_ADD_FILE:
                        cout << "\nEnter file name: ";
                        getline(cin >> ws, fileName);
                        cout << "Enter value for file: ";
                        getline(cin >> ws, fileValue);

                        if (verifyPath(path + fileName)) {
                            cout << "\n[INFO] File exists! Aborting...\n";
                            return;
                        }

                        add(fileValue, "file", fileName, path);
                        break;

                    case KEY_EDIT:
                        if (selectedPathChoice == 0) 
                            break;
                            
                        path = getPath(nodePtr, selectedPathChoice);                
                        nodePtr = locateNode(this->root, path, true);
                        string parentPath = nodePtr->path;
                        string newName;

                        cout << "\33[?25l";
                        cout << "\nEnter new name for file/folder: ";
                        getline(cin >> ws, newName);
                        cout << "\33[?25h";

                        if (nodePtr->type == "dir")
                            newName += "/";

                        nodePtr = nodePtr->parentNode;
                        renameFile(path, newName);
                        path = nodePtr->path;
                        break;
                }
            }
        }
    }
};

#endif
