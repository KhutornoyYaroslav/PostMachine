#include <iostream>
#include <conio.h>

#include "PostMachine.h"
#include "Debugger.h"
#include "Programm.h"
#include "Help.h"

using namespace std;

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");
	bool debug = false;

	if (argc == 1)
	{
		cout << HELP_INFO << endl;
		_getch();
		return -1;
	}
	if (argc < 4)
	{
		cerr << "Error. Too few arguments \n";
		_getch();
		return -1;
	}
	if (argc > 5)
	{
		cerr << "Error. Too much arguments \n";
		_getch();
		return -1;
	}
	if (argc == 5)
	{
		if (!strcmp(argv[4], "-d"))
		{
			debug = true;
		}
	}

	PostMachine machine;
	PostMachine::ErrorCode machine_result = machine.init_tape(argv[2]);
	if (machine_result != PostMachine::ErrorCode::none)
	{
		cerr << "Machine result: code " << (int)machine_result << endl;
		_getch();
		return -1;
	}

	Programm program;
	Programm::ErrorCode programm_result = program.read_from_file(argv[1]);
	if (programm_result != Programm::ErrorCode::none)
	{
		cerr << "Program result: code " << (int)programm_result << endl;
		_getch();
		return -1;
	}

	Debugger debugger(&machine, &program, argv[3]);
	Debugger::ErrorCode debugger_result;
	if (debug)
	{
		debugger_result = debugger.debug_programm();
	}
	else
	{
		debugger_result = debugger.run_programm();
	}	

	cerr << "Debugger result: code " << (int)debugger_result << endl;
	_getch();
	return 0;
}

