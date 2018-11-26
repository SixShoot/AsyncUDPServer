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
#include "exoSystem.h"

volatile sig_atomic_t flag = 0;

extern std::vector<exoModule*> exoModules;

exoSystem* pExo_system;

// ������� ��������� ������
struct command
{

	using args_type = std::vector<std::string>;

	virtual void exec(args_type const& args) const = 0;

	virtual ~command() {}
};
//------------------------------------------------------

// ������� ������ ��������� ����������
struct print_command : command
{

	virtual void exec(args_type const& args) const override
	{
		std::cout << "----------------------------" << std::endl;
		if (args.size() == 0)
		{
			std::cout << "��� ����������" << std::endl;
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
				for (int i = 0; i < ActuatorSize; i++)
				{
					std::cout << "Actuator: " << i << " Name: " << pExo_system->Actuator[i].GetName() << " Position: " << pExo_system->Actuator[i].GetCurrentPosition() << std::endl;
				}
				std::cout << std::endl;
				/*
				for (int i = 0; i < ActuatorSize; i++)
				{
					std::cout << "Motor: " << i << " Name: " << pExo_system-> exoMotors[i]->GetName()  << std::endl;
				}
				*/

			}
			else
			{
				std::cout << "�������� ��������" << std::endl;
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
					std::cout << "Actuator: " << pExo_system->Actuator[Namber].GetName() << " Position: " << pExo_system->Actuator[Namber].GetCurrentPosition() << std::endl;
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
			std::cout << "�������� ���������� ����������" << std::endl;
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

// ������
struct help_command : command
{
	virtual void exec(args_type const& args) const override
	{
		std::cout << "----------------------------" << std::endl;
		if (args.size() == 1)
		{
			if (args[0] == "print")
			{
				std::cout << "������ ���������� ����������. \n��������: \n\nprint -m - �������� ��� ��������� ������ \nprint -a 0 - �������� ��������� ������� \"A\"" << std::endl;
				std::cout << "print -m 0" << std::endl;
			}
		}
		else
		{
			std::cout << "��� ��������� �������� �� ������������ ������� ��������: help <��� �������> \n\nprint - ����� ���������� ����������; \n " << std::endl;
		}
	}
};
//------------------------------------------------------

// 
struct set_command : command
{
	virtual void exec(args_type const& args) const override
	{
		if (args.size() == 3)
		{
			if (args[0] == "-a")
			{
				int Number = 0;
				std::istringstream ist(args[1]);
				ist >> Number;

				float angle = 0;
				std::istringstream ist2(args[2]);
				ist2 >> angle;
				
				pExo_system->Actuator[Number].SetTargetPosition(angle);
			}
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
	commandMap["set"] = command_ptr(new set_command);
	
}
