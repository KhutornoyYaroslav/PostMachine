#include "Programm.h"

Programm::Programm()
{
	m_position = m_programm.begin();
}

Programm::~Programm()
{
}

bool Programm::read_next(Command &command)
{
	// Если программа пустая - читать нечего
	if (m_programm.empty() || m_position == m_programm.end())
	{
		return false;
	}
	else
	{
		// Читаем текущую команду и смещаем указатель на следующую
		command = *m_position;
		m_position = std::next(m_position);
		return true;
	}
};

bool Programm::go_to(size_t line_number)
{
	// Если программа не пустая и строка с указанным номером существует
	if (!m_programm.empty() && line_number < m_programm.size())
	{
		std::vector<Command>::iterator new_poisition = m_programm.begin();
		std::advance(new_poisition, line_number);
		m_position = new_poisition;
		return true;
	}
	else
	{	
		return false;
	}
};

Programm::ErrorCode Programm::read_from_file(const char *filename)
{
	std::ifstream input(filename);

	if (input.is_open())
	{
		while (!input.eof())
		{
			size_t line_number = {};
			std::string line;
			std::getline(input, line);
			
			size_t first_space_id = line.find_first_of(" ");

			if (first_space_id != std::string::npos)
			{
				std::string s_line_number = line.substr(0, first_space_id);
			
				if (s_line_number.find_first_not_of("0123456789") == std::string::npos && !s_line_number.empty())
				{
					line_number = std::stoi(s_line_number);
					size_t second_space_id = line.find_first_not_of(" ", first_space_id + 1);
					size_t third_space_id = line.find_first_of(" ", second_space_id + 1);
					std::string s_command = line.substr(second_space_id, third_space_id - second_space_id);

					if (s_command == "->")
					{
						m_programm.push_back({Command::CommandType::move_right, 0});
					}
					else if (s_command == "<-")
					{
						m_programm.push_back({ Command::CommandType::move_left, 0 });
					}
					else if (s_command == "0")
					{
						m_programm.push_back({ Command::CommandType::write_0, 0 });
					}
					else if (s_command == "1")
					{
						m_programm.push_back({ Command::CommandType::write_1, 0 });
					}
					else if (s_command == "GOTO")
					{
						size_t fourth_space_id = line.find_first_not_of(" ", third_space_id);
						if (fourth_space_id != std::string::npos)
						{
							size_t fifth_space_id = line.find_first_of(" ", fourth_space_id + 1);
							std::string s_parameter = line.substr(fourth_space_id, fifth_space_id - fourth_space_id);
							if (s_parameter.find_first_not_of("0123456789") == std::string::npos && !s_parameter.empty())
							{
								int parameter = std::stoi(s_parameter);
								m_programm.push_back({ Command::CommandType::go_to, parameter });
							}
							else
							{
								return ErrorCode::bad_parameter;
							}
						}
					}
					else
					{
						return ErrorCode::bad_command;
					}
				}
				else
				{
					return ErrorCode::bad_line_num;
				}		
			}
			else
			{
				return ErrorCode::bad_command;
			}
		}
		
		m_position = m_programm.begin();
		input.close();
		return ErrorCode::none;
	}
	else
	{
		return ErrorCode::bad_file;
	}
};