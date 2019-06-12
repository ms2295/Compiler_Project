/*
Name: Manjot Singh
Date: December 5, 2018
Class: CS280
Professor: Gerard Ryan
Assignment: Program 4 Part 1 Main File
*/

#include "tokens.h"
#include "parse.h"
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
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using namespace std;

map<string, Value> vars;

void RunTimeError(string msg)
{
    cerr << "0: RUNTIME ERROR " << msg << endl;
    exit (1);
}

int
main(int argc, char *argv[])
{
    ifstream file;
    istream *in;
    int linenum = 0;
    
    // CHECK ARGUMENTS, OPEN FILES
    
    if (argc == 1) // no arguments are passed
    {
        in = &cin;
        linenum = 0;
    }
    
    else if (argc == 2) // one argument is passed
    { 
        file.open(argv[1]);
            
        if (file.is_open() == 1)
        {
            in = &file;
            linenum = 0;
        }
        
        else
        {
            cout << "COULD NOT OPEN " << argv[1];
            cout << '\n';
            return 0;
        }
    }
    
    else
    {
        cout << "TOO MANY FILENAMES";
        cout << '\n';
        return 0;
    }
    
    ParseTree *prog = Prog(in, &linenum);
    
    if( prog == 0 )
    {
        return 0; // quit on error
    }
    
    // GENERATE OUTPUT
    
    prog->Eval(vars);

    return 0;
}
