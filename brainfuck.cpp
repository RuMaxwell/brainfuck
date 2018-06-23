#include "src/interpreter.hpp"

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
			interpreter::read_program(argv[2]);
			interpreter::execute();
		}
		else if (arg == "ns")
		{
			interpreter::stop_on_print = false;
			interpreter::repl();
		}
		else if (arg == "debug")
		{
			interpreter::stop_on_print = false;
			interpreter::read_program(argv[2]);
			interpreter::debug();
		}
		else
		{
			interpreter::stop_on_print = false;
			interpreter::read_program(argv[1]);
			interpreter::execute();
		}
	}
	else
	{
		interpreter::repl();
	}

	return 0;
}
