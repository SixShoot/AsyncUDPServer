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
	std::string smName[10] = { "A", "B","C" ,"D" ,"E" ,"F" ,"G" ,"H" ,"I" ,"J" };

	//�urrentSolver = EMPTY; // ������������� ������ ��������
	�urrentSolver = PATTERNS_DATA;

	Nucleo = &GetExoModule("Nucleo"); // �������� ��������� �� ������ Nucleo
	
	// �����������
	ExoMonitor.Init(GetExoModule("PC"));
	
	for (int i = 0; i < ActuatorSize; i++)
	{
		// ������������� ��������
		#ifdef USE_VREP
			Sensor[i].Init(smName[i], GetExoModule("Nucleo"));
		#else
			
			if (((i >> 1) << 1) == i) { /* ������ */  
				Sensor[i].Init(smName[i], GetExoModule("TeensyL"));			}
			else { /* �������� */   
				Sensor[i].Init(smName[i], GetExoModule("TeensyR"));			}
	
		#endif // USE_VREP


		// ������������� ����������
		Motors[i].Init(smName[i], GetExoModule("Nucleo"));

		// ������������� ��������
		Actuator[i].Init(smName[i], Motors[i], Sensor[i]);
	}
	
	pattern::ServicePatterns sPatterns;
	sPatterns.Name = "������";
	sPatterns.OpenPatterns("E:\\Antipov\\C++\\AsyncUDPServer\\AsyncUDPServer\\AsyncUDPServer\\Patterns\\������.txt");
	PatternList.push_back(sPatterns);

	sPatterns.Name = "�����";
	sPatterns.OpenPatterns("E:\\Antipov\\C++\\AsyncUDPServer\\AsyncUDPServer\\AsyncUDPServer\\Patterns\\�����.txt");
	PatternList.push_back(sPatterns);

	sPatterns.Name = "������ ���";
	sPatterns.OpenPatterns("E:\\Antipov\\IK_EXO_3D\\Pattern_Sit.txt");
	PatternList.push_back(sPatterns);
	

	// ������������� ����
	PowerOn_Handle = Nucleo->server_pack.init<uint8_t>("ULN");
}
//------------------------------------------------------------------------------------------------------
void exoSystem::SetCurrentPosition()
{
	for (int i = 0; i < ActuatorSize; i++)
	{
		Actuator[i].SetTargetPosition(Actuator[i].GetCurrentPosition());
	}
}
//-------------------------------------------------------------------------------------------------------
void exoSystem::SetInitPosition()
{
	Actuator[0].SetTargetPosition(0);
	Actuator[1].SetTargetPosition(0);
	Actuator[2].SetTargetPosition(0);
	Actuator[3].SetTargetPosition(0);
	Actuator[4].SetTargetPosition(8);
	Actuator[5].SetTargetPosition(8);
	Actuator[6].SetTargetPosition(0);
	Actuator[7].SetTargetPosition(0);
	Actuator[8].SetTargetPosition(0);
	Actuator[9].SetTargetPosition(0);
}
//------------------------------------------------------------------------------------------------------
// ������ ExoSystem
void exoSystem::run()
{
	
	// �������� ����������� ���� ��������
	while (1)
	{
		if (GetStutusConnectAllModules()) break;
		boost::this_thread::sleep(boost::posix_time::millisec(200));
	}
	
	SetCurrentPosition();
	
	boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::ptime mst2 = mst1;

	boost::posix_time::time_duration msdiff;

	while (1)
	{

		msdiff = mst2 - mst1;
		ControlFlow(msdiff.total_milliseconds());
		mst2 = boost::posix_time::microsec_clock::local_time();

		boost::this_thread::sleep(boost::posix_time::millisec(2));
	}
}
//---------------------------------------------------------------------------------------------------------
void exoSystem::SetPowerOn(uint8_t pon)
{
	Nucleo->server_pack.set(PowerOn_Handle, pon);
}
//---------------------------------------------------------------------------------------------------------
bool exoSystem::GetStutusConnectAllModules()
{
	int count = 0;
	for (int i = 0; i < exoModules_.size(); i++) if (exoModules_[i]->GetConnectStatus() || exoModules_[i]->GetName() == "PC") count++;

	if (count == exoModules_.size()) return true;
	else return false;
}
//---------------------------------------------------------------------------------------------------------
void exoSystem::StopAll() // ���������� ��.
{
	SetPowerOn(0);
	for (int i = 0; i < ActuatorSize; i++)
	{
		Motors[i].SetPWM(0);
		Motors[i].SetDirection(0, 0);
		Actuator[i].SetTargetPosition(0);
	}
}
//---------------------------------------------------------------------------------------------------------
void exoSystem::ControlFlow(uint32_t t)
{
	// ��������� ������ ����������� ���� �������
	if (GetStutusConnectAllModules() && (ERROR_ == NOERROR_))
	{		
		
		#ifndef USE_VREP
			for (int i = 0; i < ActuatorSize; i++)
			{
				Actuator[i].PIDRegulator(t);
				
				// ������ �� ������ �������
				if (Actuator[i].DriveProtection(t))
				{
					ERROR_ = DRIVEPROTECTION;
					StopAll();
				}

				if (Actuator[i].ERROR_ != 0)
				{
					ERROR_ = SENSORERROR;
					StopAll();
				}
				
				
			}
		#endif // USE_VREP


		// ����� �������� ��������
		switch (�urrentSolver)
		{
		case TXTDATA:			 // �������� ������������ �� ����� txt (�������)

			break;
		case INVERSE_KINEMATICS: // �������� ������������ �� ������� �������� ������ ����������
			IK.GetCurrentAngles(Actuator, ActuatorSize, t);
			break;
		case PATTERNS_DATA:		 // �������� ������������ �� ����� ��������� exolite txt
			if(CurrentIndexPattern != -1)
				PatternList[CurrentIndexPattern].GetCurrentAngles(Actuator, ActuatorSize, t / 100.0);
			break;
		default:
			StopAll();
			break;
		}
		
		
		SetPowerOn(1); // �������� ����	
		ExoMonitor.SendData(Actuator,t);
	}
	else
	{
		StopAll();
	}

}
//---------------------------------------------------------------------------------------------------------
exoSystem::~exoSystem()
{

}