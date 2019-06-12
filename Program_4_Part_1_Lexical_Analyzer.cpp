/*
Name: Manjot Singh
Date: December 5, 2018
Class: CS280
Professor: Gerard Ryan
Assignment: Program 4 Part 1 Lexical Analyzer File
*/

#include <iostream>
#include <stdlib.h>
#include <cctype>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <fstream>
#include <map>
#include <iterator>
#include "tokens.h"

Token getNextToken(istream *in, int *linenum)
{
    enum LexState { BEGIN, INID, INCONST, INSTRING, ONE_EQUAL, ONE_EXCLAMATION, ONE_GREATER_THAN, ONE_LESS_THAN, ONE_AND, ONE_OR, COMMENT };
    LexState lexstate = BEGIN;
    string lexeme;
    char ch;
    while(in->get(ch)) 
    {
        if( ch == '\n' )
        {
            (*linenum)++;
        }

        switch( lexstate )
        {
        case BEGIN:
            if( isspace(ch) )
            {
                continue;
            }

            lexeme += ch;

            if( isalpha(ch) )
            {
                lexstate = INID;
            }

            else if( isdigit(ch) )
            {
                lexstate = INCONST;
            }

            else if( ch == '"' )
            {
                lexstate = INSTRING;
            }

            else if( ch == '+' )
            {
                return Token(PLUS, lexeme, *linenum);
            }

            else if( ch == '-' )
            {
                return Token(MINUS, lexeme, *linenum);
            }

            else if( ch == '*' )
            {
                return Token(STAR, lexeme, *linenum);
            }

            else if( ch == '/' )
            {
                return Token(SLASH, lexeme, *linenum);
            }

            else if( ch == '(' )
            {
                return Token(LPAREN, lexeme, *linenum);
            }

            else if( ch == ')' )
            {
                return Token(RPAREN, lexeme, *linenum);
            }

            else if( ch == ';' )
            {
                return Token(SC, lexeme, *linenum);
            }

            else if( ch == '=' )
            {
                lexstate = ONE_EQUAL;
            }

            else if( ch == '!' )
            {
                lexstate = ONE_EXCLAMATION;
            }

            else if( ch == '>' )
            {
                lexstate = ONE_GREATER_THAN;
            }

            else if( ch == '<' )
            {
                lexstate = ONE_LESS_THAN;
            }

            else if( ch == '&' )
            {
                lexstate = ONE_AND;
            }

            else if( ch == '|' )
            {
                lexstate = ONE_OR;
            }

            else if( ch == '#' )
            {
                lexeme.clear();
                lexstate = COMMENT;
            }

            else
            {
                return Token(ERR, lexeme, *linenum);
            }

            break;

        case INID:
            if(lexeme == "print")
            {
                if (ch != '\n')
                {
                    in->putback(ch);
                }
                return Token(PRINT, lexeme, *linenum);
            }
                
            else if(lexeme == "if")
            {
                if (ch != '\n')
                {
                    in->putback(ch);
                }
                return Token(IF, lexeme, *linenum);
            }
                
            else if(lexeme == "then")
            {
                if (ch != '\n')
                {
                    in->putback(ch);
                }
                return Token(THEN, lexeme, *linenum);
            }
                
            else if(lexeme == "true")
            {
                if (ch != '\n')
                {
                    in->putback(ch);
                }
                return Token(TRUE, lexeme, *linenum);
            }
                
            else if(lexeme == "false")
            {
                if (ch != '\n')
                {
                    in->putback(ch);
                }
                return Token(FALSE, lexeme, *linenum);
            }
                
            lexeme += ch;
                
            if( isalpha(ch) || isdigit(ch) )
            {
                lexstate = INID;
            }

            else
            {
                if (ch != '\n')
                {
                    in->putback(ch);
                }
                lexeme.pop_back();
                return Token(IDENT, lexeme, *linenum);
            }

            break;

        
        case INCONST:
                
            lexeme += ch;

            if( isdigit(ch) )
            {
                lexstate = INCONST;
            }
                
            else if ( isalpha(ch) )
            {
                if (ch != '\n')
                {
                    in->putback(ch);
                }
                return Token(ERR, lexeme, *linenum);
            }

            else
            {
                if (ch != '\n')
                {
                    in->putback(ch);
                }
                lexeme.pop_back();
                return Token(ICONST, lexeme, *linenum);
            }

            break;

        case INSTRING:

            if( ch == '"' )
            {
                lexeme.erase(0,1);
                return Token(SCONST, lexeme, *linenum);
            }
                
            lexeme += ch;
                
            if( ch == '\n' )
            {              
                return Token(ERR, lexeme, *linenum);          
            }

            else
            {
                lexstate = INSTRING;
            }

            break;

        case ONE_EQUAL:
            lexeme += ch;
            
            if( ch == '=' )
            {
                return Token(EQ, lexeme, *linenum);
            }
                
            else
            {
                if (ch != '\n')
                {
                    in->putback(ch);
                }
                lexeme.pop_back();
                return Token(ASSIGN, lexeme, *linenum);
            }

            break;

        case ONE_EXCLAMATION:
            lexeme += ch;
            
            if( ch == '=' )
            {
                return Token(NEQ, lexeme, *linenum);
            }
                
            else
            {
                if (ch != '\n')
                {
                    in->putback(ch);
                }
                lexeme.pop_back();
                return Token(ERR, lexeme, *linenum);
            }

            break;

        case ONE_GREATER_THAN:
            lexeme += ch;

            if( ch == '=' )
            {
                return Token(GEQ, lexeme, *linenum);
            }

            else
            {
                if (ch != '\n')
                {
                    in->putback(ch);
                }
                lexeme.pop_back();
                return Token(GT, lexeme, *linenum);
            }

            break;

        case ONE_LESS_THAN:
            lexeme += ch;

            if( ch == '=' )
            {
                return Token(LEQ, lexeme, *linenum);
            }

            else
            {
                if (ch != '\n')
                {
                    in->putback(ch);
                }
                lexeme.pop_back();
                return Token(LT, lexeme, *linenum);
            }

            break;

        case ONE_AND:
            lexeme += ch;

            if( ch == '&' )
            {
                return Token(LOGICAND, lexeme, *linenum);
            }

            else
            {
                if (ch != '\n')
                {
                    in->putback(ch);
                }
                lexeme.pop_back();
                return Token(ERR, lexeme, *linenum);
            }

            break;

        case ONE_OR:
            lexeme += ch;

            if( ch == '|' )
            {
                return Token(LOGICOR, lexeme, *linenum);
            }

            else
            {
                if (ch != '\n')
                {
                    in->putback(ch);
                }
                lexeme.pop_back();
                return Token(ERR, lexeme, *linenum);
            }

            break;

        case COMMENT:

            if( ch == '\n')
            {
                char nextChar = in->peek();
                ch = nextChar;
                lexstate = BEGIN;
            }

            else
            {
                lexstate = COMMENT;
            }

            break;
        }
    }
    return Token(DONE, lexeme, *linenum);
}