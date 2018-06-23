/*
 * Project: brainfuck
 * Author: Ruthenium Maxwell, Jun 2018
 *
 * config.hpp
 *  Data structure and the interpretation of configuration file.
 * */

#include "libraries.hpp"

/*
 * Static structure that contains all configurable items.
 * */
struct config
{
    // operator_vector
    //
    // 8 chars defining the meaningful lexeme of Brainfuck language.
    // Default: +-<>[],.
    // User definition should also follow this sequence.
	static char opvec[8];

    // repl_initializing_stop_on_print
    //
    // A boolean value determine whether to pause when the program prints something. In REPL, this
    //  is used to determines its updating mode.
    //
    // If set to true, everytime the program or the interpreter prints an output, it will pause for
    //  the use to hit a key. This is used when the REPL is in clear-updating mode, which means the
    //  REPL clear the console every time it finishes a command.
    // If set to false, there will be no pause after printing. If the interpreter is running in REPL,
    //  the REPL will not clear the screen after executing a command.
	static bool repl_init_stop_on_print;
};

char config::opvec[8];
bool config::repl_init_stop_on_print;

/*
 * Read config from configuration file (namely, a file named "config" down the same directory of
 *  the executable).
 *
 * By standard, the config file should be written in the format of <property>: <value>. Whitespaces
 *  are neglected.
 * */
void read_config()
{
	string contents = read_file("config");
	string line = "";
	if (contents.empty())
	{
		return;
	}
	else
	{
		for (int i = 0; i < contents.length();)
		{
			line = take_while_not(contents, '\n', i, &i);
			i++;
			if (!line.empty())
			{
				string property = take_while_not(drop_while(line, is_whitespace), ':');
				int c = line.find_first_of(':') + 1;
				string value = drop_while(line.substr(c), is_whitespace);

				if (property == "operator_vector")
				{
					for (int j = 0; j < 8; j++)
					{
						config::opvec[j] = value[j];
					}
				}
				else if (property == "repl_initializing_stop_on_print")
				{
					config::repl_init_stop_on_print = value == "false" ? false : true;
				}
			}
			else
			{
				break;
			}
		}
	}
}
