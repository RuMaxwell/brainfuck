/*
 * Project: brainfuck
 * Author: Ruthenium Maxwell, Jun 2018
 *
 * interpreter.hpp
 *  The complete implementation of Brainfuck interpreter.
 * */

#include "config.hpp"

/*
 * Global initializer. The very first function to invoke if uses the interpreter.
 * It sets the screen size and the configurations.
 * */
void global_init()
{
	// setupterm(NULL, fileno(stdout), (int*)0);

	// Default configuration
	config::repl_init_stop_on_print = true;

	config::opvec[0] = '+';
	config::opvec[1] = '-';
	config::opvec[2] = '<';
	config::opvec[3] = '>';
	config::opvec[4] = '[';
	config::opvec[5] = ']';
	config::opvec[6] = ',';
	config::opvec[7] = '.';

	read_config();
}


// The interpreter static class
class interpreter
{
public:
	typedef map<char, void(*)()> omap;

	static char opvec[8];
	static omap op;

	static void map_init()
	{
		op[opvec[0]] = add;
		op[opvec[1]] = sub;
		op[opvec[2]] = movel;
		op[opvec[3]] = mover;
		op[opvec[4]] = loops;
		op[opvec[5]] = loope;
		op[opvec[6]] = input;
		op[opvec[7]] = output;
	}

	static bool ops(char c)
	{
		for (omap::iterator i = op.begin(); i != op.end(); i++)
		{
			if (c == i->first)
			{
				return true;
			}
		}
		return false;
	}

	static int size;
	static unsigned char* memory;

	static int ptr;

	static string program;
	static int pc;

	static list<int> ss;

	static bool stop_on_print;

	static bool debugging;

	static void initialize(int _size = 256)
	{
		size = _size;
		memory = new unsigned char[size];

		ptr = 0;

		program = "";
		pc = -1;

		for (int i = 0; i < size; i++)
		{
			memory[i] = '\0';
		}

		// Configures
		for (int i = 0; i < 8; i++)
		{
			opvec[i] = config::opvec[i];
		}

		stop_on_print = config::repl_init_stop_on_print;

		map_init();

		cout << "Brainfuck Interpreter, by Ruthenium Maxwell.\nVersion 0.2" << endl;
	}

	static void add()
	{
		memory[ptr]++;
	}

	static void sub()
	{
		memory[ptr]--;
	}

	static void movel()
	{
		ptr--;
		if (ptr < 0)
		{
			error("pointer out of memory", ptr);
		}
	}

	static void mover()
	{
		ptr++;
		if (ptr >= size)
		{
			error("pointer out of memory", ptr);
		}
	}

	static void loops()
	{
		ss.push_back(pc);
	}

	static void loope()
	{
		if (!ss.empty())
		{
			if (memory[ptr] != 0)
			{
				pc = ss.back();
			}
			else
			{
				ss.pop_back();
			}
		}
	}

	static void input()
	{
		if (stop_on_print)
		{
			getchar();
		}
		memory[ptr] = getchar();
	}

	static void output()
	{
		cerr << memory[ptr];
		if (stop_on_print)
		{
			getchar();
			getchar();
		}
	}

	static void step()
	{
		if (ops(program[pc]))
		{
			op[program[pc]]();
		}
		else
		{
			// Characters other than those eight operators are considered as comments
			// fail("Invalid character", pc);
		}
	}

	static void execute()
	{
		for (pc = 0; pc < program.length(); pc++)
		{
			step();
		}
	}

	static string& prompt(const string& msg = "")
	{
		if (msg.empty())
		{
			cerr << "bf> ";
		}
		else
		{
			cout << msg;
			cout.flush();
		}
		cin >> program;
		return program;
	}

	static void updateui()
	{
		if (stop_on_print)
		{
			system("clear");
		}
		display();
	}

	static void display()
	{
		int wid = terminal_width();

		for (int i = ptr - (int)(wid / 8); i < ptr + (int)(wid / 8); i++)
		{
			if (i < 0 || i >= size)
			{
				printf("\033[40;34m ###\033[0m");
			}
			else if (i == ptr)
			{
				printf("\033[47;30m>%3d\033[0m", memory[i]);
			}
			else
			{
				printf("%4d", memory[i]);
			}
		}

		cout << endl;
	}

	static void read_program(const char* file)
	{
		program = read_file(file);
	}

	static void repl()
	{
		while (prompt() != "q")
		{
			if (program == "r")
			{
				initialize();
				updateui();
			}
			else if (program == "l")
			{
				string file = prompt("Loading file name: ");
				read_program(file.c_str());
				execute();
			}
			else if (program == "ns")
			{
				stop_on_print = false;
			}
			else if (program == "s")
			{
				stop_on_print = true;
			}
			else if (program == "p")
			{
				cerr << "Current unit: " <<  ptr << endl;
			}
			else
			{
				execute();
				updateui();
			}
		}
	}

	static void debug()
	{
		bool ignore_blank = true;
		bool fast_mode = false;
		int step_counter = 0;

		int lastlf = 0;
		string showpiece = take_while_not(program, '\n');

		for (pc = 0; pc < program.length(); pc++)
		{
			step();

			if (program[pc] == '\n')
			{
				lastlf = pc + 1;
				showpiece = take_while_not(program, '\n', lastlf);
			}
			else if (ignore_blank && !ops(program[pc]))
			{
				continue;
			}
			else if (fast_mode
				&& program[pc] == program[pc + 1]
				&& (program[pc] == opvec[0]
				 || program[pc] == opvec[1]
				 || program[pc] == opvec[2]
				 || program[pc] == opvec[3]))
			{
				continue;
			}
			else
			{
				display();
			}

			for (int j = 0; j < showpiece.length(); j++)
			{
				if (pc == lastlf + j)
				{
					printf("\033[47;30m%c\033[0m", showpiece[j]);
				}
				else
				{
					putchar(showpiece[j]);
				}
			}

			if (step_counter > 0)
			{
				step_counter--;
			}
			else
			{
				//char cmd[256];
				string s = "";
				getline(cin, s);
				if (s == "q" || s == "quit" || s == "exit")	// quit
				{
					break;
				}
				else if (s == "p" || s == "ptr")	// ptr
				{
					cout << "Current ptr: " << ptr << endl;
				}
				else if (s == "eb" || s == "enable blank")	// enable blank
				{
					ignore_blank = true;
				}
				else if (s == "ib" || s == "ignore blank")	// disable blank
				{
					ignore_blank = false;
				}
				else if (s.substr(0, 4) == "skip")
				{
					string num = s.substr(5);
					step_counter = atoi(num.c_str());
				}
				else if (s == "f" || s == "fast")	// enable fast mode
				{
					fast_mode = true;
				}
				else if (s == "nf" || s == "nofast")	// disable fast mode
				{
					fast_mode = false;
				}
			}
		}
	}


	static void error(const string& msg)
	{
		cerr << msg << endl;
		exit(1);
	}

	static void error(const string& msg, int src)
	{
		cerr << msg << ": " << src << endl;
		exit(1);
	}

	static void fail(const string& msg, int src)
	{
		cout << msg << " at " << src << ", namely '" << program[src] << "'." << endl;
		cout << "In the program, near\n\t" << program.substr(max(0, src - 10), min((int)program.length(), src + 10)) << endl;
		if (stop_on_print)
		{
			getchar();
			getchar();
		}
	}
};

char interpreter::opvec[8];
interpreter::omap interpreter::op;

int interpreter::size;
unsigned char* interpreter::memory;

int interpreter::ptr;

string interpreter::program;
int interpreter::pc;

list<int> interpreter::ss;

bool interpreter::stop_on_print;

bool interpreter::debugging;
