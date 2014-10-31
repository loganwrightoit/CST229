#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

// Show debug console output
bool debug = true;

// Holds YES states
vector<int> yesStates;

// Store map pointers to release later
vector<map<char, int>*> delMaps;

// Holds state maps
map<int, map<char, int>*> stateMaps;

// Current state map
map<char, int> *lastStateMap;

// The initial state (first state is always zero)
int initState = 0;

// Tracks last state for easy retrieval
int lastState = 0;

bool IsYesState(char state)
{
    return std::find(yesStates.begin(), yesStates.end(), state) != yesStates.end();
}

//
// Checks if string fits language rules.
//
bool IsStringInLanguage(char * inStr)
{
    // Set current state map to start point (should always be zero)
    lastStateMap = (stateMaps.find(initState))->second;

    //////// DEBUG CODE ////////
    if (debug)
    {
        cout << endl << "****************************" << endl;
        cout << "* Processing string:" << endl;
        cout << "*" << endl;
    }
    //////// END DEBUG ////////

    for (int idx = 0; idx < strlen(inStr); ++idx)
    {
        // Skip char if it has no transition, or change state
        auto iter = lastStateMap->find(inStr[idx]);
        if (iter != lastStateMap->end())
        {
            //////// DEBUG CODE ////////
            if (debug)
            {
                cout << "* char '" << inStr[idx] << "' transitioning from state " << lastState;
            }
            //////// END DEBUG ////////

            // Grab new state and save it
            int newState = lastState = iter->second;

            // Change to new state map
            lastStateMap = stateMaps.find(newState)->second;

            //////// DEBUG CODE ////////
            if (debug)
            {
                cout << " to " << newState << endl;
            }
            //////// END DEBUG ////////
        }
        else
        {
            //////// DEBUG CODE ////////
            if (debug)
            {
                cout << "* char '" << inStr[idx] << "' has no transition, skipping." << endl;
            }
            //////// END DEBUG ////////

            continue;
        }
    }

    //////// DEBUG CODE ////////
    if (debug)
    {
        cout << "****************************" << endl << endl;
    }

    // Check if final state value matches 'YES'
    auto iter = yesStates.begin();
    while (iter != yesStates.end())
    {
        if (*iter == lastState)
        {
            return true;
        }
        iter++;
    }

    return false;
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
    cout << endl << "Enter name of language file (example.txt): ";
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
    cout << "* Recognizer analyzing file:" << endl;
    cout << "*" << endl;

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

        cout << "* Added transition: " << beginState << char(32) << char(26) << char(32) << byte << char(32) << char(26) << char(32) << endState << endl;

        // Create beginState map if it doesn't exist
        if (stateMaps.count(beginState) == 0)
        {
            auto * temp = new map<char, int>;
            delMaps.push_back(temp);
            stateMaps.insert(std::pair<int, map<char, int>*>(beginState, temp));
        }

        // Create endState map if it doesn't exist
        if (stateMaps.count(endState) == 0)
        {
            auto * temp = new map<char, int>;
            delMaps.push_back(temp);
            stateMaps.insert(std::pair<int, map<char, int>*>(endState, temp));
        }

        // Add transition to state map
        auto iter = stateMaps.find(beginState);
        if (iter != stateMaps.end()) // Key should always be found
        {
            map<char, int> *temp = iter->second;
            (*temp).insert(std::pair<char, int>(byte, endState));
        }

        if (count == 0)
        {
            initState = beginState;
        }
    }

    cout << "*" << endl;
    cout << "* Recognizer reached end of file." << endl;
    cout << "****************************" << endl << endl;

    char str[256] = "";
    cout << "Enter a string (or EXIT to exit): ";
    cin >> str;

    // Loop until user chooses to exit
    while (strcmp(str, "EXIT") != 0)
    {
        // Check if string adheres to language rules
        if (!IsStringInLanguage(str))
        {
            cout << "FAILURE: String is not in language." << endl << endl;
        }
        else
        {
            cout << "SUCCESS: String is in language." << endl << endl;
        }

        cout << "Enter a string (or EXIT to exit): ";
        cin >> str;
    }

    //////// DEBUG CODE ////////
    if (debug)
    {
        cout << endl << "****************************" << endl;
        cout << "* Contents of map on close:" << endl;
        cout << "*" << endl;

        auto iter1 = stateMaps.begin();
        while (iter1 != stateMaps.end())
        {
            auto getMap = *iter1->second;
            auto iter2 = getMap.begin();
            while (iter2 != getMap.end())
            {
                char val = iter2->first;
                int tempEnd = iter2->second;
                cout << "* stateMap[" << iter1->first << "]: " << val << char(32) << char(26) << char(32) << tempEnd << endl;
                iter2++;
            }
            iter1++;
        }

        cout << "****************************" << endl;
    }
    //////// END DEBUG ////////

    // Cleanup dynamically-created maps
    auto iter = delMaps.begin();
    while (iter != delMaps.end())
    {
        delete *iter;
        iter++;
    }

    return 0;
}