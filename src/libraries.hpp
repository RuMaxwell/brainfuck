/*
 * Project: brainfuck
 * Author: Ruthenium Maxwell, Jun 2018
 *
 * libraries.hpp
 *  Includes, and some useful utility functions.
 * */

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>

// #include <term.h>
// #include <curses.h>

using namespace std;

/*
 * Return the longest substring from a position while not encountering a specified character.
 *
 * Parameters:
 *  s: The source string
 *  c: The specified character
 *  startpos: Take substring from this index
 *  resultpos: Give back the position right after the substring if this parameter is given
 * */
string take_while_not(const string& s, char c, int startpos = 0, int* resultpos = NULL)
{
	string res = "";
	int i = startpos;
	for (; i < s.length(); i++)
	{
		if (s[i] == c)
		{
			if (resultpos)
			{
				*resultpos = i;
			}
			break;
		}
		else
		{
			res.push_back(s[i]);
		}
	}
	return res;
}

/*
 * Return a new string without the prefix right before the first character that satisfies a given condition.
 * The condition is given by a function.
 *
 * Parameters:
 *  s: The source string
 *  f: The condition function, taking every character in the string and returns if it satisfies the condition.
 * */
string drop_while(const string& s, bool (*f)(char))
{
	int i = 0;
	for (; i < s.length() && f(s[i]); i++) {}
	return s.substr(i);
}

/*
 * Determine whether a character is an ASCII whitespace character (' ' '\t' '\n').
 * */
bool is_whitespace(char c)
{
	return c == ' ' || c == '\t' || c == '\n';
}


/*
 * Read contents from a file.
 * */
string read_file(const char* file)
{
	ifstream f;
	f.open(file);
	char buff[65536];
	string res;
	while (f.getline(buff, 65536))
	{
		res.append(string(buff));
		res.push_back('\n');
	}
	f.close();
	return res;
}

/*
 * TODO: Get the width of the terminal.
 * */
int terminal_width()
{
	// return tigetnum("cols");
	return 46;
}
