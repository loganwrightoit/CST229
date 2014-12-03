/***********************************************************
* Author:                   Logan Wright
* Date Created:             12/01/2014
* Last Modification Date:   12/XX/2014
* Lab Number:               CST 229 Lab 2
* Filename:                 Tokenizer.cpp
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

#include "Tokenizer.h"

#include <iostream>

Tokenizer::Tokenizer()
{
    ////////////////////////////////////////////////////////////////
    //                           Symbols                          //
    ////////////////////////////////////////////////////////////////

    tokenMatcher.insert(std::pair<string, TokenType>("!", Symbol));
    tokenMatcher.insert(std::pair<string, TokenType>("(", Symbol));
    tokenMatcher.insert(std::pair<string, TokenType>(")", Symbol));
    tokenMatcher.insert(std::pair<string, TokenType>("[", Symbol));
    tokenMatcher.insert(std::pair<string, TokenType>("]", Symbol));
    tokenMatcher.insert(std::pair<string, TokenType>("{", Symbol));
    tokenMatcher.insert(std::pair<string, TokenType>("}", Symbol));
    tokenMatcher.insert(std::pair<string, TokenType>(":", Symbol));
    tokenMatcher.insert(std::pair<string, TokenType>(";", Symbol));
    tokenMatcher.insert(std::pair<string, TokenType>(".", Symbol)); // For identifying floats
	tokenMatcher.insert(std::pair<string, TokenType>(",", Symbol));
	tokenMatcher.insert(std::pair<string, TokenType>("<", Symbol)); // or Operator
	tokenMatcher.insert(std::pair<string, TokenType>(">", Symbol)); // or Operator
	tokenMatcher.insert(std::pair<string, TokenType>("\"", Symbol));
	tokenMatcher.insert(std::pair<string, TokenType>("\'", Symbol));

    ////////////////////////////////////////////////////////////////
    //                          Operators                         //
    ////////////////////////////////////////////////////////////////

    tokenMatcher.insert(std::pair<string, TokenType>("%", Operator));
    tokenMatcher.insert(std::pair<string, TokenType>("*", Operator)); // or Symbol (pointer, dereference)
    tokenMatcher.insert(std::pair<string, TokenType>("+", Operator));
    tokenMatcher.insert(std::pair<string, TokenType>("-", Operator));
    tokenMatcher.insert(std::pair<string, TokenType>("/", Operator));
    tokenMatcher.insert(std::pair<string, TokenType>("=", Operator));
    

    ////////////////////////////////////////////////////////////////
    //                           Boolean                          //
    ////////////////////////////////////////////////////////////////

    tokenMatcher.insert(std::pair<string, TokenType>("true", Boolean));
    tokenMatcher.insert(std::pair<string, TokenType>("false", Boolean));

    ////////////////////////////////////////////////////////////////
    //                           Keywords                         //
    ////////////////////////////////////////////////////////////////

    // Primitives
    tokenMatcher.insert(std::pair<string, TokenType>("int", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("float", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("char", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("bool", Keyword));

    // Conditional constructs
    tokenMatcher.insert(std::pair<string, TokenType>("switch", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("case", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("default", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("if", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("else", Keyword));

    // Looping constructs
	tokenMatcher.insert(std::pair<string, TokenType>("for", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("while", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("do", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("continue", Keyword));

    // General C++ keywords
    tokenMatcher.insert(std::pair<string, TokenType>("break", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("void", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("return", Keyword));

    // Add symbols and operators as delimiters
    auto iter = tokenMatcher.begin();
    while (iter != tokenMatcher.end())
    {
        if (iter->second == Symbol || iter->second == Operator)
        {
            delimiters.append(iter->first);
        }
        ++iter;
    }
}

Tokenizer::~Tokenizer()
{
}

/**************************************************************
*   Entry:  inStr is a line of text containing keywords, identifiers,
*			and other C++ language components.
*
*    Exit:  A vector containing pairs of token strings and TokenType
*			enum values.
*
* Purpose:  This function takes an input string, which ordinarily
*			will be a line from a file, and splits it into tokens.
*
***************************************************************/
std::vector<std::pair<string, Tokenizer::TokenType>> Tokenizer::Tokenize(string inStr)
{
    std::vector<std::pair<string, TokenType>> tokens;
    size_t pos = 0, prev = 0;

	// Add new-line at end of string to make tokenizing last piece easier
	inStr.append("\n");

    while ((pos = inStr.find_first_of(delimiters, prev)) != std::string::npos)
    {
        if (pos != prev)
        {
            tokens.push_back(Tokenizer::GetPair(inStr.substr(prev, pos - prev)));
        }

        // If position is before symbol, add it
        auto iter = tokenMatcher.find(inStr.substr(pos, 1));
        if (iter != tokenMatcher.end())
        {
            tokens.push_back(Tokenizer::GetPair(iter->first));
        }

        prev = pos + 1;
    }

    return tokens;
}

/**************************************************************
*   Entry:  A string token.
*
*    Exit:  A new string-TokenType pair.
*
* Purpose:  Turns a string token into a string-TokenType pair.
*
***************************************************************/
std::pair<string, Tokenizer::TokenType> Tokenizer::GetPair(string inToken)
{
    auto iter = tokenMatcher.find(inToken);
    if (iter != tokenMatcher.end())
    {
        return std::make_pair(inToken, iter->second);
    }
    else
    {
        return std::make_pair(inToken, Invalid);
    }
}

/**************************************************************
*   Entry:  A TokenType.
*
*    Exit:  A string representation of the TokenType.
*
* Purpose:  This function is mainly for debug purposes.  It
*			prints the enum TokenType as a string representation.
*
***************************************************************/
string Tokenizer::EnumToString(TokenType type)
{
    switch (type)
    {
        case Integer:
            return "Integer";
        case Keyword:
            return "Keyword";
        case Identifier:
            return "Identifier";
        case Symbol:
            return "Symbol";
        case Operator:
            return "Operator";
        case Boolean:
            return "Boolean";
        case Invalid:
            return "Invalid";
        default:
            return "Unknown Type";
    }
}