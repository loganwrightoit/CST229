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
map<char, int> *lastStateMap;

// The initial state (based on first transition in language)
int initState = -1;

// Tracks last state for easy retrieval
int lastState = -1;

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

//
// Checks if string fits language rules.
//
bool IsStringInLanguage(char * inStr)
{
    // Set current state map to start point (should always be zero)
    lastStateMap = (stateMaps.find(initState))->second;

    for (int idx = 0; idx < strlen(inStr); ++idx)
    {
        cout << "DEBUG: Processing idx[" << idx << "/]: " << inStr[idx] << endl;

        // Skip char if it has no transition, or change state
        auto iter = lastStateMap->find(inStr[idx]);
        if (iter != lastStateMap->end())
        {
            cout << "DEBUG: char '" << inStr[idx] << "' transitioning from state " << lastState;

            // Grab new state and save it
            int newState = lastState = iter->second;

            // Change to new state map
            lastStateMap = stateMaps.find(newState)->second;
            cout << " to " << newState << endl;
        }
        else
        {
            cout << "DEBUG: char '" << inStr[idx] << "' has no transition, skipping." << endl;
            continue;
        }

        /////////// NEW CODE

        cout << "DEBUG: stateMap value found." << endl;

        //auto iter = stateMaps.find(lastStateMap->find(inStr[idx]);
        //if (iter != stateMaps.end()) // Key should always be found
        //{
        //}

        /*
        auto iter1 = stateMaps.begin();
        while (iter1 != stateMaps.end())
        {
            auto getMap = *iter1->second;
            auto iter2 = getMap.begin();
            while (iter2 != getMap.end())
            {
                char val = iter2->first;
                int tempEnd = iter2->second;
                cout << "stateMap value found at idx " << iter1->first << ": " << val << ", endState: " << tempEnd << endl;
                iter2++;
            }
        }
        iter1++;
        */
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

    cout << "* Recognizer reached end of file." << endl;
    cout << "****************************" << endl << endl;

    char str[256] = "";
    cout << "Enter a string (or STOP to exit): ";
    cin >> str;

    // Loop until user chooses to exit
    while (strcmp(str, "STOP") != 0)
    {
        // Check if string adheres to language rules
        if (!IsStringInLanguage(str))
        {
            cout << "!!!!!! FAILURE: String is not in language." << endl << endl;
        }
        else
        {
            cout << "!!!!!! SUCCESS: String is in language." << endl << endl;
        }

        cout << "Enter a string (or STOP to exit): ";
        cin >> str;
    }

    //////// DEBUG - Output map contents to console
    /*
    auto iter1 = stateMaps.begin();
    while (iter1 != stateMaps.end())
    {
        auto getMap = *iter1->second;
        auto iter2 = getMap.begin();
        while (iter2 != getMap.end())
        {
            char val = iter2->first;
            int tempEnd = iter2->second;
            cout << "stateMap value found at idx " << iter1->first << ": " << val << ", endState: " << tempEnd << endl;
            iter2++;
        }
        iter1++;
    }
    */

    // Cleanup dynamically-created maps
    auto iter = delMaps.begin();
    while (iter != delMaps.end())
    {
        delete *iter;
        iter++;
    }

    return 0;
}