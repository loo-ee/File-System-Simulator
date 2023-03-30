#include <iostream>
#include "Tree.h"

using namespace std;

int main() {
    Tree<int> *tree = new Tree<int>();

    tree->add(-1, "dir", "usr/", "/");
    tree->add(-1, "dir", "home/", "/usr/");
    tree->add(-1, "dir", "documents/", "/usr/home/");
    tree->add(-1, "dir", "bin/", "/usr/");
    tree->add(5, "file", "MyDocx.docx", "/usr/home/documents/");
    tree->add(7, "file", "settings.json", "/usr/bin/");

    tree->print();
    tree->searchNode("/usr/home/documents/MyDocx.docx");
    return 0;
}