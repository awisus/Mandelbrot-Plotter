#include <string>
#include <sstream>
#include <vector>

#include "tools.hpp"

using namespace std;

vector<string> stringwerkzeuge::zerteile(string str, char trennzeichen) {
    vector<string> temp;
    stringstream stream(str);
    string wort;

    while(getline(stream, wort, trennzeichen)) {
        temp.push_back(wort);
    }

    return temp;
}
