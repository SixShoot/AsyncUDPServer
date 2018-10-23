#pragma once

//#include <string>
//#include <iostream>
//#include <vector>
//#include <sstream>
//#include <map>
#include <unordered_map>
//#include <iterator>
//#include <Windows.h>
#include <signal.h> //  our new library

volatile sig_atomic_t flag = 0;

extern std::vector<exoModule*> exoModules;
extern std::vector<exoMotor*> exoMotors;
extern std::vector<exoActuator*> exoActuators;

// thread-safe 
boost::recursive_mutex cs;



// Базовая структура команд
struct command
{

	using args_type = std::vector<std::string>;

	virtual void exec(args_type const& args) const = 0;

	virtual ~command() {}
};
//------------------------------------------------------

// Команды вывода различной информации
struct print_command : command
{

	virtual void exec(args_type const& args) const override
	{
		std::cout << "----------------------------" << std::endl;
		if (args.size() == 0)
		{
			std::cout << "Нет параметров" << std::endl;
		}
		else if (args.size() == 1)
		{
			if (args[0] == "-m")
			{
				for (int i = 0; i < exoModules.size(); i++)
				{
					std::cout << "exoModule: " << i << " Name: " << exoModules[i]->GetName() << " FPS: " << exoModules[i]->GetFPS() << " IP: " << exoModules[i]->GetIpAddress().to_string() << " Status: " << exoModules[i]->GetStringConnectStatus() << std::endl;
				}
				std::cout << std::endl;
				for (int i = 0; i < exoActuators.size(); i++)
				{
					std::cout << "Actuator: " << i << " Name: " << exoActuators[i]->GetName() << std::endl;
				}
				std::cout << std::endl;
				for (int i = 0; i < exoMotors.size(); i++)
				{
					std::cout << "Motor: " << i << " Name: " << exoMotors[i]->GetName() << std::endl;
				}

			}
			else
			{
				std::cout << "Неверный параметр" << std::endl;
			}
		}
		else if (args.size() == 2)
		{
			int Namber = 0;
			std::istringstream ist(args[1]);
			ist >> Namber;

			while (flag == 0)
			{
				if (args[0] == "-a")
				{
					boost::recursive_mutex::scoped_lock lk(cs);
					std::cout << "Actuator: " << exoActuators[Namber]->GetName() << " Position: " << exoActuators[Namber]->GetCurrentPosition() << std::endl;
				}
				else if (args[0] == "-m")
				{
					std::cout << exoModules[Namber]->GetName() << " FPS: " << exoModules[Namber]->GetFPS() << " Size: " << exoModules[Namber]->client_pack.length() << " bytes" << " Status: " << exoModules[Namber]->GetStringConnectStatus() << std::endl;
				}
				else break;

				boost::this_thread::sleep(boost::posix_time::millisec(100));
			}
			if (flag == 1) flag = 0;

		}
		else
		{
			std::cout << "Неверное количество параметров" << std::endl;
		}
		
		/*
		for (auto const& arg : args)
		{
			/*
			while (flag == 0)
			{
				Sleep(100);
				std::cout << "dfdfd" << std::endl;
			}
			if (flag == 1) flag = 0;


			std::clog << ' ' << arg;

			
		}
		*/
	}
};
//------------------------------------------------------

// Помощь
struct help_command : command
{
	virtual void exec(args_type const& args) const override
	{
		std::cout << "----------------------------" << std::endl;
		if (args.size() == 1)
		{
			if (args[0] == "print")
			{
				std::cout << "Печать отладочной информации. \nНапример: \n\nprint -m - печатать все доступные модули \nprint -a 0 - Печатать параметры привода \"A\"" << std::endl;
				std::cout << "print -m 0" << std::endl;
			}
		}
		else
		{
			std::cout << "Для получения сведений об определенной команде наберите: help <имя команды> \n\nprint - вывод отладочной информации; \n " << std::endl;
		}
	}
};
//------------------------------------------------------

void my_function(int sig)
{ // can be called asynchronously
	flag = 1; // set flag
	signal(SIGINT, my_function);
}

using command_ptr = std::unique_ptr<command>;
using command_map = std::unordered_map<std::string, command_ptr>;
command_map commandMap;

void InitCommand()
{
	commandMap["print"] = command_ptr(new print_command);
	commandMap["help"] = command_ptr(new help_command);
	
}
