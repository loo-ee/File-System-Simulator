#pragma once
#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class Tree {

    struct TreeNode {
        T data;
        string type;
        string nodeName;
        TreeNode *parentNode;
        vector<TreeNode *> children;
    };

    struct SearchNode {
        string path;
        TreeNode *node;
    };

    TreeNode *root = nullptr;

    void addChild(TreeNode *parentNode, TreeNode *child, string parentName) {

        if (parentNode->nodeName == parentName) {
            if (parentNode->type == "file") {
                cout << "Cannot append a file to another file!\n";
                return;
            } 

            child->parentNode = parentNode;
            parentNode->children.push_back(child);
            cout << "Insertion of " << child->type << ": " << child->nodeName << " done.\n";
            return;
        }

        for (TreeNode *x: parentNode->children) {
            addChild(x, child, parentName);
        }
    }

    void printTree(TreeNode *node) {
        cout << "\nName: " << node->nodeName << endl;

        cout << "Type: " << node->type << endl;
        
        if (node->type == "file") {
            cout << "Value: " << node->data << endl;
        }
        
        if (node->parentNode)
            cout << "Parent: " << node->parentNode->nodeName << endl;

        for (TreeNode* child : node->children) {
            printTree(child);
        }
    }

    SearchNode *locateNode(TreeNode *node, string nodeName) {
        static SearchNode *foundNode = nullptr; 
        // static TreeNode *foundNode = nullptr;
        string path = "";

        if (node->nodeName == nodeName) {
            foundNode = new SearchNode();

            foundNode->node = node;
        }

        for (TreeNode *x: node->children)
            locateNode(x, nodeName);

        if (foundNode) {
            TreeNode *nodePtr = foundNode->node;

            while (nodePtr) {
                path = nodePtr->nodeName + path;
                nodePtr = nodePtr->parentNode;
            }

            foundNode->path = path;
        }

        return foundNode;
    }

public:
    Tree() {
        this->root = new TreeNode();

        this->root->type = "dir";
        this->root->nodeName = "/";
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

    void searchNode(string nodeName) {
        SearchNode *foundNode = locateNode(this->root, nodeName);

        if (!foundNode) {
            cout << "\nNOT FOUND\n";
            return;
        }

        cout << "\nFOUND\n";
        cout << "Name: " << foundNode->node->nodeName << endl;
        cout << "Path: " << foundNode->path << endl;
        cout << "Type: " << foundNode->node->type << endl;

        if (foundNode->node->type == "file") 
            cout << "Value: " << foundNode->node->data << endl;
    }
};