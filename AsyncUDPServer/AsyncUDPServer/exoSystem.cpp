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


	Nucleo = &GetExoModule("Nucleo"); // Получаем указатель на обьект Nucleo
	
	for (int i = 0; i < 10; i++)
	{
		// Инициализация сенсоров
		Sensor[i].SetModule(GetExoModule("Teensy1"));
		Sensor[i].SetName(smName[i]);
		// Инициализация двигателей
		
		Motors[i].Init(smName[i], GetExoModule("Nucleo"));

		Actuator[i].Init(smName[i], Motors[i], Sensor[i]);
	}
	
	
	//pattern::OpenPatterns("E:\\Antipov\\Walk_1720_Theta4Active\\Pattern_Step1.txt");

	PowerOn_Handle = Nucleo->server_pack.init<uint8_t>("ULN");
}
//------------------------------------------------------------------------------------------------------

// Запуск ExoSystem
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

		boost::this_thread::sleep(boost::posix_time::millisec(1));
	}
}
//---------------------------------------------------------------------------------------------------------
void exoSystem::ControlFlow(int64_t t)
{
	if (PowerOn == 0) // Выключино ли реле
	{
		if (t > 10000)
		{
			Nucleo->server_pack.set2(PowerOn_Handle, 1);
			PowerOn = 1;
		}
	}

	
	
	//LOGD << "Time: " << t;

	
	
	
	
	
	//pattern::GetCurrentAngles_Patterns(t / 100.0);
	
	
	
	
	// Устанавливаем целевые углы
	for (int i = 0; i < ActuatorSize; i++)
	{
		Actuator[i].SetTargetPosition(0);

		//exoActuators[i]->SetTargetPosition(pattern::Angle[i]);
	}
	
}
//---------------------------------------------------------------------------------------------------------
exoSystem::~exoSystem()
{

}