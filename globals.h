#pragma once

#include <iostream>
#include <windows.h>
#include <iomanip>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_SELECT 115
#define KEY_DELETE 100
#define KEY_EDIT 101

using namespace std;

enum color { 
    DARKBLUE = 1, DARKGREEN, DARKTEAL, 
    DARKRED, DARKPINK, DARKYELLOW, 
    GRAY, DARKGRAY, BLUE, 
    GREEN, TEAL, RED, 
    PINK, YELLOW, WHITE 
};

struct setColor {
   color _c;
   HANDLE _console_handle;

    setColor(color c, HANDLE console_handle)
        : _c(c), _console_handle(0) { 
        _console_handle = console_handle;
    }
};

basic_ostream<char> &operator<<
(
    basic_ostream<char> &s, 
    const setColor &ref
) {
    SetConsoleTextAttribute(ref._console_handle, ref._c);
    return s;
}