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


string take_while_not(const string& s, char c, int startpos = 0)
{
	string res = "";
	for (int i = startpos; i < s.length(); i++)
	{
		if (s[i] == c)
		{
			break;
		}
		else
		{
			res.push_back(s[i]);
		}
	}
	return res;
}

void global_init()
{
	// setupterm(NULL, fileno(stdout), (int*)0);
}

int terminal_width()
{
	// return tigetnum("cols");
	return 46;
}

class interpreter
{
public:
	typedef map<char, void(*)()> omap;

	static omap op;

	static void map_init()
	{
		op['+'] = add;
		op['-'] = sub;
		op['<'] = movel;
		op['>'] = mover;
		op['['] = loops;
		op[']'] = loope;
		op[','] = input;
		op['.'] = output;
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

		map_init();

		cout << "Brainfuck Interpreter, by Ruthenium Maxwell.\nVersion 0.1" << endl;
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
			pc = memory[ptr] == 0 ? pc : ss.back();
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

	static void readfile(const char* file)
	{
		ifstream f;
		f.open(file);
		char buff[65536];
		while (f.getline(buff, 65536))
		{
			program.append(string(buff));
			program.push_back('\n');
		}
		f.close();
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
				readfile(file.c_str());
				execute();
			}
			else if (program == "nc")
			{
				stop_on_print = false;
			}
			else if (program == "c")
			{
				stop_on_print = true;
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
		int lastlf = 0;
		string showpiece = take_while_not(program, '\n');

		for (pc = 0; pc < program.length(); pc++)
		{
			step();
			display();

			if (program[pc] == '\n')
			{
				lastlf = pc + 1;
				showpiece = take_while_not(program, '\n', lastlf);
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

			char c = getchar();
			if (c == 'q')
			{
				break;
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

map<char, void(*)()> interpreter::op;

int interpreter::size;
unsigned char* interpreter::memory;

int interpreter::ptr;

string interpreter::program;
int interpreter::pc;

list<int> interpreter::ss;

bool interpreter::stop_on_print;

bool interpreter::debugging;


int main(int argc, char* argv[])
{
	global_init();
	interpreter::initialize();

	if (argc > 1)
	{
		string arg(argv[1]);
		if (arg == "run")
		{
			interpreter::stop_on_print = false;
			interpreter::program = string(argv[2]);
			interpreter::execute();
		}
		else if (arg == "file")
		{
			interpreter::stop_on_print = false;
			interpreter::readfile(argv[2]);
			interpreter::execute();
		}
		else if (arg == "nc")
		{
			interpreter::stop_on_print = false;
			interpreter::repl();
		}
		else if (arg == "debug")
		{
			interpreter::readfile(argv[2]);
			interpreter::debug();
		}
		else
		{
			interpreter::readfile(argv[1]);
			interpreter::execute();
		}
	}
	else
	{
		interpreter::stop_on_print = true;
		interpreter::repl();
	}

	return 0;
}
