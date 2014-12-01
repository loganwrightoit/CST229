#include "StateMachine.h"

StateMachine::StateMachine()
{
}

StateMachine::~StateMachine()
{
}

Tokenizer::TokenType GetTokenType(string inStr)
{
    Tokenizer tokenizer;
    return tokenizer.Invalid;
}

/*

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

// Show debug console output
bool debug = false;

// Machine language file
ifstream file;

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

// This function validates a string against the loaded language.
bool IsStringValid(char * inStr);

// This function purges transition tables and global variables.
void ResetStateMachine();

// This function translates a machine definition into algorithms.
void PrepareStateMachine(int filePos);

// This function inserts language start positions into a vector.
void GetLanguagePos(vector<int> *vec);

int main(int argc, char* argv[])
{
    //
    //  Example language rule file
    //
    //    1       - Numbers of final states (lines to read for final states)
    //    0       - Final state
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
    
    // Open language file
    file.open(fStr);
    if (!file.is_open()) {
        cout << "ERROR: Could not open file " << fStr << ", exiting." << endl;
        return 0;
    }

    // Grab language start positions
    vector<int> langPos;
    GetLanguagePos(&langPos);

    if (debug) cout << "DEBUG: Language definitions found: " << langPos.size() << endl;

    // Ask user for string in loop until EXIT
    char str[256];
    cout << endl; // Add space before initial input
    while (1)
    {
        cout << "Enter a string (BLANK for empty string, or EXIT to exit): ";
        cin.getline(str, sizeof(str));
        cout << endl; // Insert blank line
        if (strcmp(str, "EXIT") == 0) break; // Exit loop, terminate program
        if (strcmp(str, "BLANK") == 0) str[0] = '\0'; // Replace BLANK with empty string
        
        // Iterate through words and check against languages.
        // Each word MUST be valid for at least one language.
        char* context = NULL;
        bool result = true;
        char buffer[256];
        strcpy_s(buffer, str);
        char * pch = strtok_s(buffer, " ", &context);
        while (pch != NULL)
        {
            if (strcmp(pch, "BLANK") == 0) pch = "\0"; // Replace BLANK with empty string

            // Load a language, check against every word for a match.
            // Each word MUST match at least one language.
            bool match = false;
            auto iter = langPos.begin();
            while (iter != langPos.end() && !match)
            {
                PrepareStateMachine(*iter);
                if (IsStringValid(pch)) match = true;
                ResetStateMachine();
                iter++;
            }
            if (!match)
            {
                result = false;
                cout << "\tFAILURE: '" << pch << "' is not in language(s)." << endl;
            }
            else
            {
                cout << "\tSUCCESS: '" << pch << "' is in language " << (iter - langPos.begin()) << "." << endl;
            }
            pch = strtok_s(NULL, " ", &context); // Advance to next word in string
        }
        if (!result)
        {
            cout << "\t RESULT: '" << str << "' contains words not defined in language(s)." << endl << endl;
        }
        else
        {
            cout << "\t RESULT: '" << str << "' contains all valid words in language(s)." << endl << endl;
        }
    }

    return 0;
}

/**************************************************************
*   Entry:  inStr is a character array.
*
*    Exit:  a boolean for whether input string satisfies machine
*           algorithm loaded into program.
*
* Purpose:  Validates a string against the loaded language.
*
***************************************************************/
/*
bool IsStringValid(char * inStr)
{
    lastStateMap = (stateMaps.find(initState))->second; // Set current state map to start point (usually zero)
    for (unsigned int idx = 0; idx < strlen(inStr); ++idx)
    {
        // Check if char is in language
        if (language.find(inStr[idx]) != string::npos)
        {
            auto iter = lastStateMap->find(inStr[idx]); // Skip char if it has no transition, or change state
            if (iter != lastStateMap->end())
            {
                int newState = currState = iter->second; // Grab new state and save it
                lastStateMap = stateMaps.find(newState)->second; // Change to new state map
            }
        }
        else
        {
            return false; // Char is not in language, so string is not in language
        }
    }

    // Check if final state value matches 'YES'
    auto iter = yesStates.begin();
    while (iter != yesStates.end())
    {
        if (*iter == currState) return true;
        iter++;
    }

    return false;
}

/**************************************************************
*   Entry:  none.
*
*    Exit:  none.
*
* Purpose:  Purges transition tables and global variables.
*
***************************************************************/
/*
void ResetStateMachine()
{
    yesStates.clear();
    language = "";
    initState = currState = -1;
    lastStateMap = NULL;

    // Prints map contents to console
    if (debug)
    {
        cout << endl << "****************************" << endl;
        cout << "* Transitions on machine reset:" << endl;
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

        cout << "****************************" << endl << endl;
    }

    stateMaps.clear();

    // Cleanup dynamically-created maps
    auto iter = delMaps.begin();
    while (iter != delMaps.end())
    {
        delete *iter;
        iter++;
    }
    delMaps.clear();
}

/**************************************************************
*   Entry:  filePos is where in the local variable 'file' to
*           begin reading a machine definition.
*
*    Exit:  none.
*
* Purpose:  Translates a machine definition into algorithms.
*
***************************************************************/
/*
void PrepareStateMachine(int filePos)
{
    // Seek to language position in file
    file.clear();
    file.seekg(filePos, ios::beg);

    std::string line;

    // Get number YES states as lines
    getline(file, line);
    int numYesStates = atoi(line.c_str());

    if (debug)
    {
        cout << endl << "****************************" << endl;
        cout << "* Number of states: " << numYesStates << endl;
    }

    // Store YES states
    for (int state = 0; state < numYesStates; ++state)
    {
        getline(file, line);
        yesStates.push_back(atoi(line.c_str()));
        if (debug) cout << "* Added 'YES' state: " << atoi(line.c_str()) << endl;
    }

    // Grab number of transition lines defined
    getline(file, line);
    int numTransitions = atoi(line.c_str());
    if (debug) cout << "* Number of transitions: " << numTransitions << endl;

    // Map the transitions
    for (int count = 0; count < numTransitions; ++count)
    {
        getline(file, line);
        int beginState, endState;
        char byte;

        // Copy string to cstring for tokenizer
        char buffer[256] = "";
        strcpy_s(buffer, sizeof(buffer), line.c_str());

        char *context = NULL;
        char * pch;
        pch = strtok_s(buffer, " ", &context);
        beginState = atoi(pch);
        pch = strtok_s(NULL, " ", &context);
        byte = pch[0]; // Should only be one character
        pch = strtok_s(NULL, " ", &context);
        endState = atoi(pch);

        if (debug)
        {
            if (count == 0)
                cout << "* Added transition: " << beginState << char(32) << char(26) << char(32) << byte << char(32) << char(26) << char(32) << endState << " (initial state set to " << beginState << ")" << endl;
            else
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

    if (debug) cout << "****************************" << endl;
}

/**************************************************************
*   Entry:  vec is a vector reference for storing machine
*           definition start positions in a file.
*
*    Exit:  none.
*
* Purpose:  Inserts file language start positions into a vector.
*
***************************************************************/
/*
void GetLanguagePos(vector<int> *vec)
{
    string line;
    vec->push_back((int)file.tellg());// Add language start position to array
    getline(file, line);
    bool hasData = !line.empty();
    while (getline(file, line))
    {
        int pos = file.cur;
        if (line.empty())
        {
            if (hasData)
            {
                hasData = false;
                vec->push_back((int)file.tellg());
            }
        }
        else
        {
            hasData = true;
        }
    }
}
*/