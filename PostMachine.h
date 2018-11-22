#pragma once
#include <list>
#include <iterator>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

class PostMachine
{
public:

	enum class ErrorCode
	{
		none,
		bad_first_line,
		bad_second_line,
		empty_file,
		bad_file
	};


private:
	std::list<bool> m_tape;
	std::list<bool>::iterator m_position;

public:
	PostMachine();
	~PostMachine();
		
	void move_left();
	void move_right();
	void write_0();
	void write_1();

	std::string get_position();
	std::string get_tape();
	void clear_tape();

	ErrorCode init_tape(const char *filename);
};

