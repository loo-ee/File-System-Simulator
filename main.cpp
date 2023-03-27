#include <iostream>
#include "Tree.h"

using namespace std;

int main() {
    Tree<int> *tree = new Tree<int>();

    tree->add(-1, "dir", "usr/", "/");
    tree->add(5, "file", "hello.num", "usr/");
    tree->add(-1, "dir", "bin/", "usr/");
    tree->add(6, "file", "hehe.num", "usr/");
    tree->add(7, "file", "haha.num", "bin/");

    tree->print();
    tree->searchNode("haha.num");
    tree->searchNode("bin/");
    tree->searchNode("hehe.num");

    return 0;
}