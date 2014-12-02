/***********************************************************
* Author:                   Logan Wright
* Date Created:             12/01/2014
* Last Modification Date:   12/XX/2014
* Lab Number:               CST 229 Lab 2
* Filename:                 StateMachine.cpp
*
* Overview:
*   This program is an extension to Lab 1's state machine.
*   It tokenizes a text file containing programming
*   language code (C++ in this case), and identifies each
*   token as a symbol, operator, keyword, identifier, etc.
*   A token may not match any expected type, in which case
*   it will be marked as invalid.
*
* Input:
*   The user must provide a text file name containing
*   the program code, such as: code1.txt
*
* Output:
*   The program will analyze the file and display all
*   tokens found with a description.
*
*       Example:    Token               Token Type
*                   for                 Keyword
*                   while               Keyword
*                   _test               Identifier
*                   @                   Invalid
*                   12                  Integer
************************************************************/

#include "StateMachine.h"

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

StateMachine::StateMachine()
{
    // Add language definitions
    definitions.push_back(std::make_pair("int-def.txt", Tokenizer::TokenType::Integer));
    definitions.push_back(std::make_pair("ident-def.txt", Tokenizer::TokenType::Identifier));
}

StateMachine::~StateMachine()
{
    ResetStateMachine();
}

/**************************************************************
*   Entry:  token is the string to match.
*
*    Exit:  a TokenType for matching language type, or Invalid
*           if no match was found.
*
* Purpose:  Matches a token against a list of language
*           definitions to identify whether it matches a
*           TokenType.
*
***************************************************************/
Tokenizer::TokenType StateMachine::GetTokenType(string token)
{
    //  Example language definition file
    //
    //    1       - Numbers of final states (lines to read for final states)
    //    0       - Final state
    //    ...     - More final states...
    //    2       - Number of transitions (lines to read for transition definitions)
    //    0 a 1   - Transition where state 0 goes to 1 if input is 'a' (first state is initial state)
    //    1 a 0   - More transitions...
    //

    // Cycle through language files looking for a match
    auto iter = definitions.begin();
    while (iter != definitions.end())
    {
        PrepareStateMachine(iter->first);
        if (IsStringValid(token))
        {
            return iter->second;
        }
        ResetStateMachine();

        ++iter;
    }

    return Tokenizer::TokenType::Invalid;
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
bool StateMachine::IsStringValid(string token)
{
    lastStateMap = (stateMaps.find(initState))->second; // Set current state map to start point (usually zero)
    for (unsigned int idx = 0; idx < token.length(); ++idx)
    {
        // Check if char is in language
        if (language.find(token.at(idx)) != string::npos)
        {
            auto iter = lastStateMap->find(token.at(idx)); // Skip char if it has no transition, or change state
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
* Purpose:  Purges transition tables and variables.
*
***************************************************************/
void StateMachine::ResetStateMachine()
{
    yesStates.clear();
    language = "";
    initState = currState = -1;
    lastStateMap = NULL;

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
*   Entry:  The filename of the language definition file to
*           load.
*
*    Exit:  none.
*
* Purpose:  Translates a machine definition into algorithms.
*
***************************************************************/
void StateMachine::PrepareStateMachine(string fileName)
{
    // Load file
    ifstream file;
    file.open(fileName);
    if (!file.is_open()) {
        cout << "ERROR: Could not open language file " << fileName << ", skipping." << endl;
        exit;
    }

    string line;

    // Get number YES states as lines
    getline(file, line);
    int numYesStates = atoi(line.c_str());

    // Store YES states
    for (int state = 0; state < numYesStates; ++state)
    {
        getline(file, line);
        yesStates.push_back(atoi(line.c_str()));
    }

    // Grab number of transition lines defined
    getline(file, line);
    int numTransitions = atoi(line.c_str());

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
}