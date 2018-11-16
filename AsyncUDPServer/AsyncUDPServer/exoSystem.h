#pragma once


//#define USE_VREP

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
#include "ServicePatterns.hpp"
#include "exoInverseKinematics.hpp"


enum SolverType { EMPTY, TXTDATA, INVERSE_KINEMATICS, PATTERNS_DATA }; // Тип решателя


class exoSystem
{
public:
	exoActuator   Actuator[ActuatorSize];

	exoSystem(std::vector<exoModule*>& exoModules);


	void ControlFlow(int64_t t);
	exoModule& GetExoModule(std::string name); 
	bool GetStutusConnectAllModules();
	

	void SetPowerOn(uint8_t pon); // Включить реле
	void StopAll(); // Остановить всё.

	void run();


	~exoSystem();

private:

	std::vector<exoModule*>& exoModules_;

	exoSensor     Sensor[10];
	exoMotor      Motors[10];

	//Экзомодуль
	exoModule* Nucleo;

	// Служебные переменные
	int PowerOn = 0;
	uint8_t PowerOn_Handle = 0;

	pattern::ServicePatterns sPatterns;
	exoInverseKinematics IK;
	
	SolverType СurrentSolver;
};
