#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

#include <string>
using namespace std;

class InputHelper {
public:
    static int getInt(const string& prompt);
    static string getString(const string& prompt);
};

#endif