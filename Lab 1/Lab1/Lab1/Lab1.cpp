#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

// Holds YES states
vector<int> yesStates;

// Store map pointers to release later
vector<map<char, int>*> delMaps;

// Holds state maps
map<int, map<char, int>*> stateMaps;

// Current state map
map<char, int> currentStateMap;

/*
Three machines to test:

A machines that accepts an even number of 0’s. {00, 0000, 000000, ….}
A machine that accepts any integer. {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, ……}
A machine that accepts any legal C variable name. {index, xyz, name1, dept_2, ….}

*/

bool IsYesState(char state)
{
    return std::find(yesStates.begin(), yesStates.end(), state) != yesStates.end();
}

int main()
{
    //
    //  Example language rule file
    //
    //    1       - Numbers of final states (lines to read for final states)
    //    0       - Final
    //    ...     - More final states...
    //    2       - Number of transitions (lines to read for transition definitions)
    //    0 a 1   - Transition where state 0 goes to 1 if input is 'a'
    //    1 a 0   - More transitions...
    //

    // Holds YES states
    vector<int> values;
    vector<int>::iterator current;

    char fStr[256] = "";
    cout << "Enter name of language file (example.txt): ";
    cin >> fStr;
    cout << endl;

    // Read in pattern from file
    std::ifstream inFile(fStr);

    if (!inFile.is_open()) {
        cout << "Could not open file " << fStr << ", exiting." << endl;
        return 0;
    }

    std::string line;

    cout << "****************************" << endl;
    cout << "* Recognizer reading file." << endl;

    // Get number YES states as lines
    std::getline(inFile, line);
    int numYesStates = atoi(line.c_str());
    cout << "* Number of states: " << numYesStates << endl;

    // Store YES states
    for (int state = 0; state < numYesStates; ++state)
    {
        std::getline(inFile, line);
        yesStates.push_back(atoi(line.c_str()));
        cout << "* Added 'YES' state: " << atoi(line.c_str()) << endl;
    }

    // Grab number of transition lines defined
    std::getline(inFile, line);
    int numTransitions = atoi(line.c_str());
    cout << "* Number of transitions: " << numTransitions << endl;

    // Map the transitions
    for (int count = 0; count < numTransitions; ++count)
    {
        std::getline(inFile, line);
        int beginState, endState;
        char byte;

        // Copy string to cstring for tokenizer
        char buffer[256] = "";
        strcpy_s(buffer, sizeof(buffer), line.c_str());

        char *context = NULL;
        char * pch;
        pch = strtok_s(buffer, " ", &context);
        beginState = atoi(pch);               // MessageType
        pch = strtok_s(NULL, " ", &context);
        byte = pch[0]; // Should only be one character
        pch = strtok_s(NULL, " ", &context);
        endState = atoi(pch);

        cout << "* Added transition: " << beginState << char(26) << byte << char(26) << endState << endl;

        // Create state map if it doesn't exist
        if (stateMaps.count(beginState) == 0)
        {
            auto * temp = new map<char, int>;
            delMaps.push_back(temp);
            stateMaps.insert(std::pair<int, map<char, int>*>(beginState, temp));
        }

        // Add transition to state map
        auto iter = stateMaps.find(beginState);
        if (iter != stateMaps.end()) // Key should always be found
        {
            map<char, int> temp = *iter->second;
            temp.insert(std::pair<char, int>(byte, endState));
        }
    }

    cout << "* Recognizer reached end of file." << endl;
    cout << "****************************" << endl << endl;

    char str[256];
    cout << "Enter a string: ";
    cin >> str;
    cout << endl;

    cout << "DEBUG: Detected string: " << str << endl;

    // Cleanup dynamic maps
    auto iter = delMaps.begin();
    while (iter != delMaps.end())
    {
        delete *iter;
        iter++;
    }

    return 0;
}