#define _WIN32_WINNT 0x0501



#include <cstdlib>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <plog/Log.h>

#include <boost/thread.hpp>
#include <vector>
#include <clocale> //Обязательно для функции setlocale()

#include "exoUDPServer.h"
#include "exoActuator.h"
#include "exoSensor.h"
#include "exoMotor.h"
#include "ConsoleTerminal.h"

using namespace boost::posix_time;

boost::asio::io_service io_service;

//boost::shared_ptr<exoModule> pNucleo_1(new exoModule(io_service, "127.0.0.1", "Nucleo1")); //"12.0.4.239"


exoModule pModule1(io_service, "12.0.3.248", "Module1");
std::vector<exoModule*> exoModules = { &pModule1 };



exoSensor SensorA("A", pModule1);
exoSensor SensorB("B", pModule1);
exoSensor SensorC("C", pModule1);
exoSensor SensorD("D", pModule1);
exoSensor SensorE("E", pModule1);
exoSensor SensorF("F", pModule1);
exoSensor SensorG("G", pModule1);
exoSensor SensorH("H", pModule1);
exoSensor SensorI("I", pModule1);
exoSensor SensorJ("J", pModule1);
std::vector<exoSensor*> exoSensors = { &SensorA };

exoMotor MotorA("A", pModule1);
exoMotor MotorB("B", pModule1);
exoMotor MotorC("C", pModule1);
exoMotor MotorD("D", pModule1);
exoMotor MotorE("E", pModule1);
exoMotor MotorF("F", pModule1);
exoMotor MotorG("G", pModule1);
exoMotor MotorH("H", pModule1);
exoMotor MotorI("I", pModule1);
exoMotor MotorJ("J", pModule1);

std::vector<exoMotor*> exoMotors = { &MotorA };
//exoSensor SensorB("B", pModule1);
//exoModule* exoModules[1] = { &pModule1 };

//-------------------------------------------------------------------------------------------

void print(const boost::system::error_code& /*e*/, boost::asio::deadline_timer* t)
{
	t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
	t->async_wait(boost::bind(print, boost::asio::placeholders::error, t));

	/*
	for (int i = 0; i < exoModules.size(); i++)
	{
		std::cout << "Module: " << i << " FPS: " << exoModules[i]->FPS << " SensorA: " << ActuatorA.GetAngle() << std::endl;
	}
	*/
	
	
	//std::cout << "Nucleo1  FPS: " << Nucleo_1.FPS << " SensorA: " << ActuatorA.GetAngle() << std::endl;

	//std::cout << "PC  FPS: " << PCModule.FPS << std::endl;


}
//-------------------------------------------------------------------------------------------

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
//-------------------------------------------------------------------------------------------
void MainExo(const boost::system::error_code& /*e*/, boost::asio::deadline_timer* t)
{
	t->expires_at(t->expires_at() + boost::posix_time::millisec(2));
	t->async_wait(boost::bind(MainExo, boost::asio::placeholders::error, t));

	// 500 Гц.
	//********************MAIN****************************
		
		//LOGD << "Hello log!";

	//********************!MAIN!****************************
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
		
	try
	{
		exoUDPServer server(io_service, exoModules, 4442);

		boost::asio::deadline_timer exo_time(io_service, boost::posix_time::millisec(2));
		exo_time.async_wait(boost::bind(MainExo, boost::asio::placeholders::error, &exo_time));


		//boost::asio::deadline_timer t(io_service, boost::posix_time::seconds(1));
		//t.async_wait(boost::bind(print, boost::asio::placeholders::error, &t));


		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception:  " << e.what() << "\n";
		LOGE << "Exception:  " << e.what();
	}

	//thr.join();

	return 0;
}