#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

// Show debug console output
bool debug = false;

// Holds letters defined in language
string language;

// Holds YES states
vector<int> yesStates;

// Store map pointers to release later
vector<map<char, int>*> delMaps;

// Holds state maps
map<int, map<char, int>*> stateMaps;

// Current state map
map<char, int> *lastStateMap;

// The initial state (determined by first state in first transition)
int initState = -1;

// Tracks last state for easy retrieval
int currState = -1;

//
// Checks if string fits language rules.
//
bool IsStringInLanguage(char * inStr)
{
    // Set current state map to start point (usually zero)
    lastStateMap = (stateMaps.find(initState))->second;

    if (debug)
    {
        cout << endl << "****************************" << endl;
        cout << "* Processing string:" << endl;
        cout << "*" << endl;
    }

    for (unsigned int idx = 0; idx < strlen(inStr); ++idx)
    {
        // Check if char is in language
        if (language.find(inStr[idx]) != string::npos)
        {
            // Skip char if it has no transition, or change state
            auto iter = lastStateMap->find(inStr[idx]);
            if (iter != lastStateMap->end())
            {
                if (debug)
                {
                    cout << "* char '" << inStr[idx] << "' transitioning from state " << currState;
                }

                // Grab new state and save it
                int newState = currState = iter->second;

                // Change to new state map
                lastStateMap = stateMaps.find(newState)->second;

                if (debug)
                {
                    cout << " to " << newState << endl;
                }
            }
        }
        else
        {
            if (debug)
            {
                cout << "* ERROR: character '" << inStr[idx] << "' not in language" << endl;
                cout << "****************************" << endl;
            }

            // Char is not in language, so string is not in language
            return false;
        }
    }

    if (debug)
    {
        cout << "****************************" << endl;
    }

    // Check if final state value matches 'YES'
    auto iter = yesStates.begin();
    while (iter != yesStates.end())
    {
        if (*iter == currState)
        {
            return true;
        }
        iter++;
    }

    return false;
}

int main(int argc, char* argv[])
{
    //
    //  Example language rule file
    //
    //    1       - Numbers of final states (lines to read for final states)
    //    0       - Final
    //    ...     - More final states...
    //    2       - Number of transitions (lines to read for transition definitions)
    //    0 a 1   - Transition where state 0 goes to 1 if input is 'a' (first state is initial state)
    //    1 a 0   - More transitions...
    //

    char fStr[256] = "";
    
    switch (argc)
    {
    case 2:
        strcpy_s(fStr, argv[1]);
        break;
    case 3:
        strcpy_s(fStr, argv[1]);
        if (strcmp(argv[2], "debug") == 0)
        {
            debug = true;
        }
        break;
    default:
        cout << "ERROR: Incorrect arguments [args: <name>.txt (opt)debug]." << endl;
        return 1;
    }

    // Holds YES states
    vector<int> values;
    vector<int>::iterator current;

    // Read in pattern from file
    std::ifstream inFile(fStr);

    if (!inFile.is_open()) {
        cout << "Could not open file " << fStr << ", exiting." << endl;
        return 0;
    }

    std::string line;

    cout << endl << "****************************" << endl;

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

        if (debug && count == 0)
        {
            cout << "* Added transition: " << beginState << char(32) << char(26) << char(32) << byte << char(32) << char(26) << char(32) << endState << " (initial state set to " << beginState << ")" << endl;
        }
        else
        {
            cout << "* Added transition: " << beginState << char(32) << char(26) << char(32) << byte << char(32) << char(26) << char(32) << endState << endl;
        }

        // Add char to language
        if (language.find_first_of(byte) == string::npos)
        {
            language += byte;
            language.append(",");
        }

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


        // Set initial state of machine
        if (count == 0)
        {
            initState = currState = beginState;
        }
    }

    // Shave last comma from language
    language = language.substr(0, language.size() - 1);

    cout << "* Language Set: {" << language << "}" << endl;
    cout << "****************************" << endl << endl;

    char str[256] = "";
    cout << "Enter a string (BLANK for empty string, or EXIT to exit): ";
    cin >> str;

    // Loop until user chooses to exit
    while (strcmp(str, "EXIT") != 0)
    {
        if (strcmp(str, "BLANK") == 0)
        {
            str[0] = '\0';
        }

        // Check if string adheres to language rules
        if (!IsStringInLanguage(str))
        {
            cout << "\n\tFAILURE: \"" << str << "\" is not in language." << endl << endl;
        }
        else
        {
            cout << "\n\tSUCCESS: \"" << str << "\" is in language." << endl << endl;
        }

        currState = initState; // Reset state for next string
        cout << "Enter a string (BLANK for empty string, or EXIT to exit): ";
        cin >> str;
    }

    if (debug)
    {
        cout << endl << "****************************" << endl;
        cout << "* Transitions in map:" << endl;
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
                cout << "* " << iter1->first << char(32) << char(26) << char(32) << val << char(32) << char(26) << char(32) << tempEnd << endl;
                iter2++;
            }
            iter1++;
        }

        cout << "****************************" << endl;
    }

    // Cleanup dynamically-created maps
    auto iter = delMaps.begin();
    while (iter != delMaps.end())
    {
        delete *iter;
        iter++;
    }

    return 0;
}