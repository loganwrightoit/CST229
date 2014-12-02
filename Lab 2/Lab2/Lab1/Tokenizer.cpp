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

    ////////////////////////////////////////////////////////////////
    //                          Operators                         //
    ////////////////////////////////////////////////////////////////

    tokenMatcher.insert(std::pair<string, TokenType>("%", Operator));
    tokenMatcher.insert(std::pair<string, TokenType>("*", Operator));
    tokenMatcher.insert(std::pair<string, TokenType>("+", Operator));
    tokenMatcher.insert(std::pair<string, TokenType>("-", Operator));
    tokenMatcher.insert(std::pair<string, TokenType>("/", Operator));
    tokenMatcher.insert(std::pair<string, TokenType>("<", Operator));
    tokenMatcher.insert(std::pair<string, TokenType>("=", Operator));
    tokenMatcher.insert(std::pair<string, TokenType>(">", Operator));

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
    tokenMatcher.insert(std::pair<string, TokenType>("while", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("do", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("continue", Keyword));

    // General usage
    tokenMatcher.insert(std::pair<string, TokenType>("break", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("void", Keyword));
    tokenMatcher.insert(std::pair<string, TokenType>("return", Keyword));

    // Add symbols as delimiters
    auto iter = tokenMatcher.begin();
    while (iter != tokenMatcher.end())
    {
        if (iter->second == Symbol)
        {
            delimiters.append(iter->first);
        }
        ++iter;
    }

    cout << "Delimiters: " << delimiters << endl;
}

Tokenizer::~Tokenizer()
{
}

//
// Tokenizes a string and returns a vector with <String,TokenType> pairs.
//
std::vector<std::pair<string, Tokenizer::TokenType>> Tokenizer::Tokenize(string inStr)
{
    std::vector<std::pair<string, TokenType>> tokens;
    size_t pos = 0, prev = 0;

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

//
// Returns a <String,TokenType> pair.
//
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

//
// Returns string representation for TokenType enumerator.
//
string Tokenizer::EnumToString(TokenType type)
{
    switch (type)
    {
        case Integer:
            return "Integer";
        case Float:
            return "Float";
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