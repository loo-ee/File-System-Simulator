#include <iostream>
#include "Tree.h"

using namespace std;

int main() {
    FileSystem<int> *tree = new FileSystem<int>();

    tree->add(-1, "dir", "usr/", "/");
    tree->add(-1, "dir", "home/", "/usr/");
    tree->add(-1, "dir", "bin/", "/usr/");
    tree->add(1, "file", "Name.txt", "/usr/home/");
    tree->add(2, "file", "Works.txt", "/usr/home/");
    tree->add(3, "file", "settings.json", "/usr/bin/");
    tree->add(-1, "dir", "static/", "/usr/bin/");
    tree->add(4, "file", "static.json", "/usr/bin/static/");
    tree->add(-1, "dir", "img/", "/usr/bin/static/");
    tree->add(5, "file", "photo.png", "/usr/bin/static/img/");
    tree->add(-1, "dir", "favourites/", "/usr/bin/static/img/");
    tree->add(6, "file", "me.png", "/usr/bin/static/img/favourites/");
    tree->add(-1, "dir", "system/", "/usr/");
    tree->add(7, "file", "system.deb", "/usr/system/");

    tree->print(1);

    return 0;
}