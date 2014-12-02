/***********************************************************
* Author:                   Logan Wright
* Date Created:             12/01/2014
* Last Modification Date:   12/XX/2014
* Lab Number:               CST 229 Lab 2
* Filename:                 Tokenizer.h
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

#pragma once

#include <vector>
#include <map>
#include <string>

using namespace std;

class Tokenizer
{

    public:

        enum TokenType
        {
            Integer,
            Keyword,
            Identifier,
            Symbol,
            Operator,
            Boolean,
            Invalid
        };

        Tokenizer();
        ~Tokenizer();

        std::vector<std::pair<string, TokenType>> Tokenize(string inStr);
        string EnumToString(TokenType type);

    private:

        std::pair<string, TokenType> GetPair(string inToken);

        // Contains string tokens that are common to language, with an associated TokenType.
        map<string, Tokenizer::TokenType> tokenMatcher;

        // Used to discover tokens in language files
        string delimiters = "  \t";

};

