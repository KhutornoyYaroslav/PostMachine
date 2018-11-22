#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

class Programm
{
public:

	enum class ErrorCode
	{
		none,
		bad_file,
		end_prog,
		bad_command,
		bad_line_num,
		bad_parameter,
		bad_goto
	};

	struct Command
	{
		enum class CommandType
		{
			write_0,
			write_1,
			move_left,
			move_right,
			go_to
		};

		CommandType type;
		int parameter;
	};

private:
	std::vector<Command> m_programm;
	std::vector<Command>::iterator m_position;

public:
	Programm();
	~Programm();

	bool read_next(Command &command);
	bool go_to(size_t line_number);
	ErrorCode read_from_file(const char *filename);
};

