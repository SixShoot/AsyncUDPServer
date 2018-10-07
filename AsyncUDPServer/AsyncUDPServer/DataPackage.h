#pragma once


struct PK
{

};

struct Server_DataPack
{
	char MotorA_INA;
	char MotorA_INB;
	float  MotorA_PWM;
	char MotorB_INA;
	char MotorB_INB;
	float  MotorB_PWM;
};


struct Client_DataPack
{
	unsigned short SensorA;
	unsigned short SensorB;
	unsigned short SensorC;
	unsigned short SensorD;
	unsigned short SensorE;
	unsigned short SensorF;
	unsigned short SensorG;
	unsigned short SensorH;
	unsigned short SensorI;
	unsigned short SensorJ;
	unsigned short SensorK;

};

