#include "InputHelper.h"
#include <iostream>
#include <limits>
using namespace std;

int InputHelper::getInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;

        // Якщо введено не число — помилка
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Try again.\n";
        } else return value;
    }
}

string InputHelper::getString(const string& prompt) {
    string value;
    cout << prompt;
    cin >> value;
    return value;
}