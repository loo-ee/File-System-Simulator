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

    void printTree(TreeNode *node) {
        cout << "\nName: " << node->nodeName << endl;
        cout << "Type: " << node->type << endl;
        cout << "Path: " << node->path << endl;
        
        if (node->type == "file") {
            cout << "Value: " << node->data << endl;
        }
        
        if (node->parentNode)
            cout << "Parent: " << node->parentNode->path << endl;

        for (TreeNode* child : node->children) {
            printTree(child);
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
            cout << "\n[INFO] File with new name already exists in specified directory.\n";
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

    void print() {
        printTree(this->root);
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

        vector<TreeNode*> children = nodeToDelete->parentNode->children;
        

        for (int i = 0; i < children.size(); i++) {
            if (children[i]->path == path) {
                TreeNode *temp = children[i];

                children[i] = nullptr;
                delete temp;

                cout << "\n[INFO] File deleted\n";
                break;
            }
        }
    }

    void renameFile(string path, string newName) {
        editFileName(path, newName);
    }
};