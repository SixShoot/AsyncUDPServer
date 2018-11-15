#define _WIN32_WINNT 0x0501


#include "exoSystem.h"

exoModule& exoSystem::GetExoModule(std::string name)
{
	for (int i = 0; i < exoModules_.size(); i++)
	{
		if (exoModules_[i]->GetName() == name)
		{
			return *exoModules_[i];
			break;
		}
	}

	LOGE << "Error: not found exoModule: " << name;

}
//----------------------------------------------------------------------------------------------

exoSystem::exoSystem(std::vector<exoModule*>& exoModules) : exoModules_(exoModules)
{
	std::string smName[10] = { "A", "B","C" ,"D" ,"E" ,"F" ,"J" ,"H" ,"I" ,"J" };

	�urrentSolver = EMPTY; // ������������� ������ ��������


	Nucleo = &GetExoModule("Nucleo"); // �������� ��������� �� ������ Nucleo
	
	for (int i = 0; i < ActuatorSize; i++)
	{
		// ������������� ��������
		Sensor[i].Init(smName[i], GetExoModule("Teensy1"));

		// ������������� ����������
		Motors[i].Init(smName[i], GetExoModule("Nucleo"));

		// ������������� ��������
		Actuator[i].Use_VREP = true;
		Actuator[i].Init(smName[i], Motors[i], Sensor[i]);
	}
	
	sPatterns.OpenPatterns("E:\\Antipov\\Walk_1720_Theta4Active\\Pattern_Step1.txt");

	// ������������� ����
	PowerOn_Handle = Nucleo->server_pack.init<uint8_t>("ULN");
}
//------------------------------------------------------------------------------------------------------

// ������ ExoSystem
void exoSystem::run()
{
	boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::ptime mst2 = mst1;

	boost::posix_time::time_duration msdiff;

	while (1)
	{

		msdiff = mst2 - mst1;
		ControlFlow(msdiff.total_milliseconds());
		mst2 = boost::posix_time::microsec_clock::local_time();

		boost::this_thread::sleep(boost::posix_time::millisec(50));
	}
}
//---------------------------------------------------------------------------------------------------------
void exoSystem::SetPowerOn(uint8_t pon)
{
	Nucleo->server_pack.set2(PowerOn_Handle, pon);
}
//---------------------------------------------------------------------------------------------------------
void exoSystem::StopAll() // ���������� ��.
{
	for (int i = 0; i < ActuatorSize; i++)
	{
		SetPowerOn(0);
		Motors[i].SetPWM(0);
		Motors[i].SetDirection(0, 0);
		Actuator[i].SetTargetPosition(0);
	}
}
//---------------------------------------------------------------------------------------------------------
void exoSystem::ControlFlow(int64_t t)
{
	
	// ����� �������� ��������
	switch (�urrentSolver)
	{
	case TXTDATA:			 // �������� ������������ �� ����� txt (�������)
		
		break;
	case INVERSE_KINEMATICS: // �������� ������������ �� ������� �������� ������ ����������
		IK.GetCurrentAngles(Actuator, ActuatorSize, t);
		break;
	case PATTERNS_DATA:		 // �������� ������������ �� ����� ��������� exolite txt
		sPatterns.GetCurrentAngles(Actuator, ActuatorSize, t / 100.0);

		break;
	default:
		StopAll();
		break;
	}
	

	/*
	// ������������� ������� ����
	for (int i = 0; i < ActuatorSize; i++)
	{
		Actuator[i].SetTargetPosition(sPatterns.Angle[i]);
	}
	*/

	/*
	SetPowerOn(1);

	Motors[8].SetDirection(0, 1);
	Motors[8].SetPWM(10);
	*/

	
}
//---------------------------------------------------------------------------------------------------------
exoSystem::~exoSystem()
{

}