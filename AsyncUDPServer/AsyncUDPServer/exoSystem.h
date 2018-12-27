#pragma once


#define USE_VREP

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
#include "exoMonitor.hpp"


enum SolverType { EMPTY, TXTDATA, INVERSE_KINEMATICS, PATTERNS_DATA }; // Тип решателя

enum ErrorType { NOERROR_, DRIVEPROTECTION, SENSORERROR, ERROR2, ERROR3}; // Тип решателя


class exoSystem
{
public:
	exoActuator   Actuator[ActuatorSize];
	
	std::vector<pattern::ServicePatterns> PatternList;
	int CurrentIndexPattern = -1;


	exoSystem(std::vector<exoModule*>& exoModules);


	void ControlFlow(uint32_t t);
	exoModule& GetExoModule(std::string name); 
	bool GetStutusConnectAllModules();

	void SetInitPosition();
	void SetCurrentPosition();

	void SetPowerOn(uint8_t pon); // Включить реле
	void StopAll(); // Остановить всё.

	void run();


	~exoSystem();

private:

	ErrorType ERROR_ = NOERROR_;

	std::vector<exoModule*>& exoModules_;

	exoSensor     Sensor[10];
	exoMotor      Motors[10];

	//Экзомодуль
	exoModule* Nucleo;
	
	// Служебные переменные
	int PowerOn = 0;
	uint8_t PowerOn_Handle = 0;

	
	exoInverseKinematics IK;
	
	SolverType СurrentSolver;

	exoMonitor ExoMonitor;
};
