#pragma once


#define ActuatorSize 10

#include <cstdlib>
#include <iostream>
#include <plog/Log.h>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>


#include "exoSensor.hpp"
#include "exoMotor.hpp"
#include "exoActuator.hpp"

//#include "ServicePatterns.hpp"


//extern std::vector<exoModule*> exoModules;
//extern std::vector<exoActuator*> exoActuators;



class exoSystem
{
public:
	exoActuator   Actuator[ActuatorSize];

	exoSystem(std::vector<exoModule*>& exoModules);


	void ControlFlow(int64_t t);
	exoModule& GetExoModule(std::string name); 

	void run();


	~exoSystem();

private:

	std::vector<exoModule*>& exoModules_;

	exoSensor     Sensor[10];
	exoMotor      Motors[10];

	//Ёкзомодуль
	exoModule* Nucleo;

	// —лужебные переменные
	int PowerOn = 0;
	uint8_t PowerOn_Handle = 0;
	

};
