/***********************************************************
* Author:                   Logan Wright
* Date Created:             12/01/2014
* Last Modification Date:   12/XX/2014
* Lab Number:               CST 229 Lab 2
* Filename:                 Lab2.cpp
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

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include "Tokenizer.h"
#include "StateMachine.h"

using namespace std;

// Program code file
ifstream file;

// Stores token and type
vector<std::pair<string, Tokenizer::TokenType>> tokens;

int main(int argc, char* argv[])
{
    string fileName;
    switch (argc)
    {
    case 2:
        fileName.append(argv[1]);
        break;
    default:
        cerr << "ERROR: argument <filename> required." << endl;
        return 1;
    }

    // Open file
    file.open(fileName);
    if (!file.is_open()) {
        cerr << "ERROR: could not open file " << fileName << ", exiting." << endl;
        return 1;
    }

    // DEBUG
    cout << "File opened." << endl;

    // Create tokenizer
    Tokenizer tokenizer;

    // DEBUG
    cout << "Tokenizer created." << endl;

    // Create state machine
    StateMachine stateMachine;

    // DEBUG
    cout << "StateMachine created." << endl;

    // Begin analyzing file
    std::string line;
    while (std::getline(file, line))
    {
        // DEBUG
        cout << "Analyzing line: " << line << endl;

        std::vector<std::pair<string, Tokenizer::TokenType>> result = tokenizer.Tokenize(line);

        auto iter = result.begin();
        while (iter != result.end())
        {
            // If type is Invalid, check against language definitions
            //if (iter->second == tokenizer.Invalid)
            //{
            //    iter->second = stateMachine.GetTokenType(iter->first);
            //}
            
            // Store token and type pair
            tokens.push_back(*iter);
            ++iter;
        }
    }

    // Output results to console
    cout << right << setw(20) << "Token" << setw(20) << "Token Type" << endl;
    cout << setw(40) << setfill('-') << "-" << setfill(' ') << endl;
    auto iter = tokens.begin();
    while (iter != tokens.end())
    {
        cout << setw(20) << iter->first << setw(20) << tokenizer.EnumToString(iter->second) << endl;
        ++iter;
    }
}