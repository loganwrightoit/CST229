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
            Float,
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

        map<string, Tokenizer::TokenType> tokenMatcher;
        string delimiters = "  \t";

};

