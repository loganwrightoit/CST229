/***********************************************************
* Author:                   Logan Wright
* Date Created:             12/01/2014
* Last Modification Date:   12/02/2014
* Lab Number:               CST 229 Lab 2
* Filename:                 StateMachine.h
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

#include "Tokenizer.h"

class StateMachine
{

    public:

        StateMachine();
        ~StateMachine();

		/**********************************************************************
		*	Tokenizer::TokenType GetTokenType(string token);
		*
		*	  	Purpose:	Gets the TokenType that represents the given string.
		*
		*		Entry:		A string token.
		*
		*		Exit:		A TokenType enum.
		*
		***********************************************************************/

        Tokenizer::TokenType GetTokenType(string token);

    private:

		/**********************************************************************
		*	bool IsStringValid(string token);
		*
		*		Purpose:	Validates a string against the loaded language.
		*
		*		Entry:		A string token.
		*
		*		Exit:		a boolean for whether input string satisfies machine
		*					algorithm loaded into program.
		*
		*	void ResetStateMachine();
		*
		*	  	Purpose:	Purges transition tables and variables.
		*
		*		Entry:		none.
		*
		*		Exit:		None.
		*
		*	void PrepareStateMachine(string fileName);
		*
		*	  	Purpose:	Translates a machine definition into algorithms.
		*
		*		Entry:		The filename of the language definition file to
		*					load.
		*
		*		Exit:		none.
		*
		***********************************************************************/

        bool IsStringValid(string token);
        void ResetStateMachine();
        void PrepareStateMachine(string fileName);

        // Holds <filename,TokenType> pair for language definitions
        std::vector<std::pair<string, Tokenizer::TokenType>> definitions;

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

};

