#include "utilities.h"
#include <iostream>
using namespace std;

string read_string(string prompt)
{
    cout << prompt;
    string result;
    getline(cin >> ws, result); // '>> ws' discards leftover enter keys automatically!
    return result;
}

int read_integer(string prompt)
{
    cout << prompt;
    int result;
    cin >> result;
    return result;
}