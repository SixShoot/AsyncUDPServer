#pragma once


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
//#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <cstdlib>
#include <iostream>
#include "exoSensor.hpp"
#include "exoMotor.hpp"
#include "exoSystem.h"


class exoActuator
{
public:

	exoActuator()
	{

	}
	//-----------------------------------------------------------------------------
	void CreateDefaultConfig()
	{
		try
		{
			pt.put("Sensor.in_min", 0);
			pt.put("Sensor.in_max", 0);
			pt.put("Sensor.out_min", 0);
			pt.put("Sensor.out_max", 0);
			pt.put("PIDRegulator.Kp", 1.0);
			pt.put("PIDRegulator.Ki", 0);
			pt.put("PIDRegulator.Kd", 0);
			boost::property_tree::ini_parser::write_ini("config\\Actuator" + name_ + ".ini", pt);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
			LOGE << e.what();
		}
	}
	//-----------------------------------------------------------------------------
	void LoadConfig()
	{
		boost::filesystem::path p{ "config" };

		if (!boost::filesystem::exists("config")) boost::filesystem::create_directory("config");
		if (!boost::filesystem::exists("config\\Actuator" + name_ + ".ini")) CreateDefaultConfig();
		else
		{
			// Загружаем параметры из файла
			try
			{
				boost::property_tree::ini_parser::read_ini("config\\Actuator" + name_ + ".ini", pt);

				in_min = pt.get<float>("Sensor.in_min");
				in_max = pt.get<float>("Sensor.in_max");
				out_min = pt.get<float>("Sensor.out_min");
				out_max = pt.get<float>("Sensor.out_max");

				Kp = pt.get<float>("PIDRegulator.Kp");
				Ki = pt.get<float>("PIDRegulator.Ki");
				Kd = pt.get<float>("PIDRegulator.Kd");

			}
			catch (std::exception& e)
			{
				std::cout << e.what() << std::endl;
				LOGE << e.what();
			}
		}
	}
	//---------------------------------------------------------------------------
	void Init(std::string Name, exoMotor& motor, exoSensor& sensor)
	{
		motor_  = &motor;
		sensor_ = &sensor;
		name_   = Name;

		// Загружаем параметры из ini файла
		LoadConfig();
	}

	//-----------------------------------------------------------------------------
	float map(float x, float in_min, float in_max, float out_min, float out_max)
	{
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
	//-----------------------------------------------------------------------------
	void SetTargetPosition_VREP(float angle)
	{
		if (angle < 0)  motor_->SetDirection(1, 0);
		if (angle > 0)  motor_->SetDirection(0, 1);
		if (angle == 0) motor_->SetDirection(1, 1);

		float U = fabs(angle);
		motor_->SetPWM(U);
	}
	//-----------------------------------------------------------------------------
	void PIDRegulator()
	{
		float error = (GetCurrentPosition() - TargetAngle);

		if (error < 0)  motor_->SetDirection(1, 0);
		if (error > 0)  motor_->SetDirection(0, 1);
		if (error == 0) motor_->SetDirection(1, 1);

		float U = fabs(error) * Kp;

		if (U > 100) U = 100;

		motor_->SetPWM(U);
	}
	//-----------------------------------------------------------------------------
	void SetTargetPosition(float angle)
	{
		// Если используем симулятор
		#ifdef USE_VREP
			SetTargetPosition_VREP(angle);
			return;
		#else
			//PIDRegulator(angle);
			TargetAngle = angle;
		#endif // USE_VREP
	}
	//----------------------------------------------------------------------------------
	float GetCurrentPosition()
	{
		//return sensor_->GetValue();//map(sensor_.GetValue(),1,1,1,10);
		//float angle = map(sensor_->GetValue(), in_min, in_max, out_min, out_max);
		return map(sensor_->GetValue(),in_min,in_max,out_min,out_max);
	}
	//----------------------------------------------------------------------------------
	std::string GetName()
	{
		return name_;
	}
	//----------------------------------------------------------------------------------
	~exoActuator()
	{

	}

private:
	std::string name_;

	exoMotor*  motor_;
	exoSensor* sensor_;

	boost::property_tree::ptree pt; // Для работы с файлами настроек

	// Параметры регулятора
	float Kp;
	float Ki;
	float Kd;

	// Параметры для мапа
	float in_min;
	float in_max;
	float out_min;
	float out_max;

	float TargetAngle = 0;

};