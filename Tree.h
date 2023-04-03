#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cctype>

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

    void printFileTree(TreeNode *node, int tabCount) {
        static TreeNode *previousNode = nullptr;
        static TreeNode *currentParent = nullptr;
        TreeNode *nodePtr = currentParent;
        int counter = 1;

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
            cout << "--> " << node->nodeName << "\n";
        }

        tabCount++;
        previousNode = node;

        if (node->children.size() == 0) 
            return; 

        if (node->type == "file")
            currentParent = node->parentNode;

        for (TreeNode* child : node->children) {
            printFileTree(child, tabCount);
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

    void print(int mode) {
        switch (mode) {
            case 0:
                cout << "\n[PRINTING CMD MODE]\n\n";
                print_CMD_mode(this->root);
                break;

            case 1:
                cout << "\n[PRINTING TREE]\n\n";
                printFileTree(this->root, 0);
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
                TreeNode *temp = children->at(i);

                children->erase(children->begin() + i);
                delete temp;

                cout << "\n[INFO] File deleted\n";
                break;
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

    string traverseFileSystem(string path, char mode, bool deleteStatus) {
        int choice = 0;
        string selectedPath = "";

        TreeNode *nodePtr = locateNode(this->root, path, true);

        switch (mode) {
            case 'd':
                if (deleteStatus) {
                    if (!verifyPath(path)) {
                        cout << "\n[WARNING] Invalid choice\n";
                        selectedPath = nodePtr->path;
                    }
                    else {
                        nodePtr = nodePtr->parentNode;
                        deleteFile(path);
                        
                    }

                    deleteStatus = false;
                }

                break;

            case 'e':
                string parentPath = nodePtr->path;
                string newName;

                cout << "\nEnter new name for file/folder: ";
                getline(cin >> ws, newName);

                renameFile(parentPath, newName);
                break;
        } 

        cout << "\n[Select Directory or File]\n";
        cout << "[DIR] " << nodePtr->path << endl;
        cout << "[0] ../ \n";

        if (nodePtr->type == "dir") {
            if (nodePtr->children.size() == 0) {
                cout << "\n[INFO] No more sub directories\n";
            }

            for (int i = 1; i <= nodePtr->children.size(); i++) {
                cout << "[" << i << "] " << nodePtr->children[i -1]->nodeName << endl;
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

        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 0 && nodePtr->parentNode) 
            selectedPath = nodePtr->parentNode->path;
        else if (choice == 0 && !nodePtr->parentNode)
            return "";
        else {
            if ((choice -1) >= nodePtr->children.size() || (choice -1) < 0)
                selectedPath = nodePtr->path;
            else
                selectedPath = nodePtr->children[choice -1]->path;
        }

        return selectedPath;
    }
};