#include "Debugger.h"

Debugger::Debugger(PostMachine *post_machine, Programm *programm, const char* result_file_name)
{
	m_programm = programm;
	m_machine = post_machine;
	m_output_file.open(result_file_name); 
}

Debugger::~Debugger()
{
}

bool Debugger::run_command(Programm::Command command)
{
	switch (command.type)
	{
	case Programm::Command::CommandType::move_left:
		m_machine->move_left();
		break;

	case Programm::Command::CommandType::move_right:
		m_machine->move_right();
		break;

	case Programm::Command::CommandType::write_0:
		m_machine->write_0();
		break;

	case Programm::Command::CommandType::write_1:
		m_machine->write_1();
		break;

	case Programm::Command::CommandType::go_to:
		if (!m_programm->go_to(command.parameter))
		{
			return false;
		}		
		break;

	default:
		return false;
		break;
	}

	return true;
};

Debugger::ErrorCode Debugger::run_programm()
{
	// Запуск программы без отладки
	Programm::Command command = {};
	size_t step_counter = 0;

	// Считываем, пока не закончится программа
	while (m_programm->read_next(command))
	{		
		if (run_command(command))
		{
			print_step(&m_output_file, command, step_counter);
			++step_counter;
		}
		else
		{
			return ErrorCode::end_of_programm;
		}
	}

	return ErrorCode::none;
};

Debugger::ErrorCode Debugger::debug_programm()
{
	// Запуск программы в режиме отладки
	std::cout << HELP_DEBUG << std::endl;

	Debugger::ErrorCode result = Debugger::ErrorCode::none;
	size_t step_counter = 0;
	
	// Считываем значения нажатых клавиш и выполняем указанные действия
	while (true)
	{
		Programm::Command command = {};
	
		switch (_getch())
		{
		// Если нажали клавишу "n" - next step
		case 110: case 226: case 78: case 146: 

			if (m_programm->read_next(command))
			{
				if (!run_command(command))
				{
					return Debugger::ErrorCode::bad_command;
				}
				else
				{
					print_step(&std::cout, command, step_counter);
					print_step(&m_output_file, command, step_counter);
					++step_counter;
				}
			}
			else
			{
				print_step(&std::cout, command, step_counter);
				print_step(&m_output_file, command, step_counter);
				return Debugger::ErrorCode::end_of_programm;
			}
			break;

		// Если нажали клавишу "s" - stop
		case 115: case 83: case 155: case 235: 

			return Debugger::ErrorCode::none;
			break;

		// Если нажали клавишу "g" - go
		case 103: case 71: case 143: case 175:

			while (m_programm->read_next(command))
			{
				if (!run_command(command))
				{
					return Debugger::ErrorCode::bad_command;
				}
				else
				{
					print_step(&std::cout, command, step_counter);
					print_step(&m_output_file, command, step_counter);
					++step_counter;
				}
			}

			return Debugger::ErrorCode::end_of_programm;
			break;

		default:
			break;
		}
	}
};


void Debugger::print_step(std::ostream *out_stream, Programm::Command cmd, size_t step_number)
{
	*out_stream << "step: " << step_number << " ";

	switch (cmd.type)
	{
	case Programm::Command::CommandType::move_left:
		
		*out_stream << "command: <-";
		break;

	case Programm::Command::CommandType::move_right:
		
		*out_stream << "command: ->";
		break;

	case Programm::Command::CommandType::write_0:
		
		*out_stream << "command: write 0";
		break;

	case Programm::Command::CommandType::write_1:
		
		*out_stream << "command: write 1";
		break;

	case Programm::Command::CommandType::go_to:

		*out_stream << "command: GOTO" << cmd.parameter;
		break;

	default:
		break;
	}

	*out_stream << std::endl;
	*out_stream << m_machine->get_position() << std::endl;
	*out_stream << m_machine->get_tape() << std::endl << std::endl;
}