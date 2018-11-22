#pragma once
#include <conio.h>

#include "PostMachine.h"
#include "Programm.h"
#include "Help.h"

class Debugger
{
public:
	enum class ErrorCode
	{
		none,
		bad_command,
		end_of_programm
	};

private:
	PostMachine *m_machine;
	Programm *m_programm;
	std::ofstream m_output_file;

public:
	Debugger(PostMachine *post_machine, Programm *programm, const char* result_file_name);
	~Debugger();
	
	ErrorCode run_programm();
	ErrorCode debug_programm();

private:
	bool run_command(Programm::Command command);	
	void print_step(std::ostream *out_stream, Programm::Command cmd, size_t step_number);
};

