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

#include "exoPackage.hpp"
#include "ServicePatterns.hpp"

using namespace boost::posix_time;

boost::asio::io_service io_service;

exoModule pModule_Teensy1(io_service, "192.168.0.103", "Teensy1");
exoModule pModule_Nucleo(io_service, "192.168.0.100", "Nucleo");
std::vector<exoModule*> exoModules = { &pModule_Teensy1 , &pModule_Nucleo };



exoSensor SensorA("A", pModule_Teensy1);
exoSensor SensorB("B", pModule_Teensy1);
exoSensor SensorC("C", pModule_Teensy1);
exoSensor SensorD("D", pModule_Teensy1);
exoSensor SensorE("E", pModule_Teensy1);
exoSensor SensorF("F", pModule_Teensy1);
exoSensor SensorG("G", pModule_Teensy1);
exoSensor SensorH("H", pModule_Teensy1);
exoSensor SensorI("I", pModule_Teensy1);
exoSensor SensorJ("J", pModule_Teensy1);
exoSensor SensorCMPS1("CMPS1", pModule_Teensy1);
exoSensor SensorCMPS2("CMPS2", pModule_Teensy1);


std::vector<exoSensor*> exoSensors = { &SensorA };

exoMotor MotorA("A", pModule_Nucleo);
exoMotor MotorB("B", pModule_Nucleo);
exoMotor MotorC("C", pModule_Nucleo);
exoMotor MotorD("D", pModule_Nucleo);
exoMotor MotorE("E", pModule_Nucleo);
exoMotor MotorF("F", pModule_Nucleo);
exoMotor MotorG("G", pModule_Nucleo);
exoMotor MotorH("H", pModule_Nucleo);
exoMotor MotorI("I", pModule_Nucleo);
exoMotor MotorJ("J", pModule_Nucleo);

std::vector<exoMotor*> exoMotors = { &MotorA };


exoActuator ActuatorA("A", MotorA, SensorA);
exoActuator ActuatorB("B", MotorB, SensorB);
exoActuator ActuatorC("C", MotorC, SensorC);
exoActuator ActuatorD("D", MotorD, SensorD);
exoActuator ActuatorE("E", MotorE, SensorE);
exoActuator ActuatorF("F", MotorF, SensorF);
exoActuator ActuatorG("G", MotorG, SensorG);
exoActuator ActuatorH("H", MotorH, SensorH);
exoActuator ActuatorI("I", MotorI, SensorI);
exoActuator ActuatorJ("J", MotorJ, SensorJ);

std::vector<exoActuator*> exoActuators = { &ActuatorA , &ActuatorB, &ActuatorC, &ActuatorD, &ActuatorE, &ActuatorF, &ActuatorG, &ActuatorH, &ActuatorI, &ActuatorJ };

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
//-------------------------------------------------------------------------------------------
uint32_t GlobalTime = 0;
void MainExo(const boost::system::error_code& /*e*/, boost::asio::deadline_timer* t)
{
	t->expires_at(t->expires_at() + boost::posix_time::millisec(2));
	t->async_wait(boost::bind(MainExo, boost::asio::placeholders::error, t));

	// 500 Гц.
	//********************MAIN****************************
	GlobalTime += 2;
	pattern::GetCurrentAngles_Patterns(GlobalTime / 100.0);
	for (int i = 0; i < exoActuators.size(); i++)
	{
		//exoActuators[i]->SetTargetPosition(45.0);
		exoActuators[i]->SetTargetPosition(pattern::Angle[i]);
		
	}

	if (GlobalTime > 400) GlobalTime = 0;

		//LOGD << "Hello log!";

	//********************!MAIN!****************************
}
//----------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{

	pattern::OpenPatterns("E:\\Antipov\\Walk_1720_Theta4Active\\Pattern_Step1.txt");
	
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