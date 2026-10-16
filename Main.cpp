#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
using namespace std;
string readText(const string& msg) {
    string s;
    do {
        cout << msg;
        if (!getline(cin, s)) { cout << "\nInput ended.\n"; exit(0); }
    } while (s.empty());
    return s;
}

double readNumber(const string& msg, double lo, double hi) {
    while (true) {
        string s = readText(msg);
        stringstream ss(s); double v; char extra;
        if (ss >> v && !(ss >> extra) && v >= lo && v <= hi) return v;
        cout << "Enter a value between " << lo << " and " << hi << ".\n";
    }
}

string toLower(string s) {
    for (char& c : s) c = tolower((unsigned char)c);
    return s;
}
