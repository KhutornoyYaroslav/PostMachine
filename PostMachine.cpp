#include "PostMachine.h"

PostMachine::PostMachine()
{
	clear_tape();
};

PostMachine::~PostMachine()
{
};

void PostMachine::move_left()
{
	// Создаем новую ячейку в ленте и переходим в нее
	if (m_position == m_tape.begin())
	{
		m_tape.push_front(false);
		m_position = m_tape.begin();
	}
	else // Если ячейка уже есть
	{
		m_position = std::prev(m_position);
	}
};

void PostMachine::move_right()
{
	std::list<bool>::iterator new_position = std::next(m_position);

	// Создаем новую ячейку в ленте и переходим в нее
	if (new_position == m_tape.end())
	{
		m_position = m_tape.insert(new_position, false);
	}
	else // Если ячейка уже есть
	{
		m_position = new_position;
	}
};

void PostMachine::write_0()
{
	*m_position = false;
};

void PostMachine::write_1()
{
	*m_position = true;
};

PostMachine::ErrorCode PostMachine::init_tape(const char *filename)
{
	std::ifstream input(filename);

	if (input.is_open())
	{
		int tape_position = -1;
		std::string first_line;
		std::getline(input, first_line);

		// Парсинг первой строки с положением каретки
		if (!first_line.empty())
		{
			for (size_t i = 0; i < first_line.size(); ++i)
			{
				if (first_line.at(i) == 'v')
				{
					tape_position = i;
				}
				else if (first_line.at(i) != ' ')
				{
					return ErrorCode::bad_first_line;
				}
			}
		}
		else
		{
			return ErrorCode::empty_file;
		}

		if (tape_position == -1)
		{
			return ErrorCode::bad_first_line;
		}

		std::string second_line;
		std::getline(input, second_line);

		// Парсинг второй строки с значениями ленты
		if (!second_line.empty())
		{
			for (size_t i = 0; i < second_line.size(); ++i)
			{
				if (second_line.at(i) == '0') 
				{
					write_0();
					if(i < second_line.size() - 1)
						move_right();								
				}
				else if(second_line.at(i) == '1')
				{
					write_1();
					if (i < second_line.size() - 1)
						move_right();				
				}
				else
				{
					clear_tape();
					return ErrorCode::bad_second_line;
				}
			}
		}
		else
		{
			return ErrorCode::bad_second_line;
		}	

		std::list<bool>::iterator new_position = m_tape.begin();
		std::advance(new_position, tape_position);
		m_position = new_position;
		input.close();

		return ErrorCode::none;
	}
	else
	{
		return ErrorCode::bad_file;
	}
};

void PostMachine::clear_tape()
{
	m_tape.clear();
	m_position = m_tape.insert(m_tape.begin(), false);
};

std::string PostMachine::get_position()
{
	// Возращаем текущее положение каретки в виде строки
	std::string result;

	for (auto it = m_tape.begin(); it != m_tape.end(); ++it)
	{
		if (m_position == it)
		{
			result.append("v");
		}
		else
		{
			result.append(" ");
		}
	}

	return result;
};

std::string PostMachine::get_tape()
{
	// Возращаем текущее состояние ленты в виде строки
	std::string result;

	for (auto it = m_tape.begin(); it != m_tape.end(); ++it)
	{
		if (*it == true)
		{
			result.append("1");
		}
		else
		{
			result.append("0");
		}		
	}

	return result;
}