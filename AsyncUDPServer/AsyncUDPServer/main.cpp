#define _WIN32_WINNT 0x0501


#include <cstdlib>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <plog/Log.h>

#include <boost/thread.hpp>
#include <vector>
#include <clocale> //Обязательно для функции setlocale()


#include "exoSystem.h"
#include "exoUDPServer.h"
#include "ConsoleTerminal.h"

using namespace boost::posix_time;


boost::asio::io_service io_service;


#ifdef USE_VREP
	exoModule pModule1(io_service, "127.0.0.1", "Nucleo");
	std::vector<exoModule*> exoModules = { &pModule1 };
#else
	exoModule pModule1(io_service, "192.168.0.102", "TeensyR");
	exoModule pModule2(io_service, "192.168.0.103", "TeensyL");
	exoModule pModule3(io_service, "192.168.0.101", "Nucleo");
	std::vector<exoModule*> exoModules = { &pModule1 , &pModule2 , &pModule3 };
#endif // USE_VREP

//-----------------------------------------------------------------------------------------------------------------------

void ThreadTerminal()
{
	setlocale(LC_ALL, "Russian");

	// Register signals 
	signal(SIGINT, my_function);

	InitCommand();

	for (std::string line; std::getline(std::cin, line); )
	{
		std::istringstream iss(line);

		std::string commandName;
		iss >> commandName;

		auto const it = commandMap.find(commandName);
		if (it != std::end(commandMap))
		{
			command::args_type const commandArgs(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{});
			it->second->exec(commandArgs);
		}
		else
		{
			std::cerr << "'" << commandName << "' - Unknown command" << std::endl;
		}
	}
}

//----------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	// Initialize the logger that will be measured.
	plog::init(plog::debug, "logs\\log.txt", 1000000, 10); 


	if (argc == 2)
	{
		if(!strcmp(argv[1], "-T"))
			boost::thread TT(ThreadTerminal);
	}
	
	exoSystem exo_system(exoModules);
	// Для терминала
	pExo_system = &exo_system;

	boost::thread thread1([&exo_system]() { exo_system.run(); }); // Запускаем в отдельном потоке


	try
	{
		exoUDPServer server(io_service, exoModules, 4442);
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception:  " << e.what() << "\n";
		LOGE << "Exception:  " << e.what();
	}

	thread1.join();
	
	return 0;
}