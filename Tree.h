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
            if (locateNode(this->root, (path + child->nodeName))) {
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

    TreeNode *locateNode(TreeNode *node, string path) {
        if (node->path == path)
            return node;

        for (TreeNode *x: node->children)
            return locateNode(x, path);

        return nullptr;
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
        TreeNode *foundNode = locateNode(this->root, path);

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
};