#include <iostream>
#include "Tree.h"

using namespace std;

int main() {
    FileSystem<int> *tree = new FileSystem<int>();

    tree->add(-1, "dir", "usr/", "/");
    tree->add(-1, "dir", "home/", "/usr/");
    tree->add(-1, "dir", "documents/", "/usr/home/");
    tree->add(5, "file", "MyDocx.docx", "/usr/home/documents/");
    tree->add(-1, "dir", "bin/", "/usr/");
    tree->add(7, "file", "settings.json", "/usr/bin/");

    tree->print();

    return 0;
}