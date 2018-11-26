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

	//СurrentSolver = EMPTY; // Устанавливаем пустой решатель
	СurrentSolver = PATTERNS_DATA;

	Nucleo = &GetExoModule("Nucleo"); // Получаем указатель на обьект Nucleo
	
	for (int i = 0; i < ActuatorSize; i++)
	{
		// Инициализация сенсоров
		#ifdef USE_VREP
			Sensor[i].Init(smName[i], GetExoModule("Nucleo"));
		#else
			Sensor[i].Init(smName[i], GetExoModule("Teensy1"));
		#endif // USE_VREP


		// Инициализация двигателей
		Motors[i].Init(smName[i], GetExoModule("Nucleo"));

		// Инициализация приводов
		Actuator[i].Init(smName[i], Motors[i], Sensor[i]);
	}
	
	sPatterns.OpenPatterns("E:\\Antipov\\Walk_1720_Theta4Active\\Pattern_Step1.txt");

	// Инициализация реле
	PowerOn_Handle = Nucleo->server_pack.init<uint8_t>("ULN");
}
//------------------------------------------------------------------------------------------------------

// Запуск ExoSystem
void exoSystem::run()
{
	
	// Ожидание подключения всех клиентов
	while (1)
	{
		if (GetStutusConnectAllModules()) break;
		boost::this_thread::sleep(boost::posix_time::millisec(200));
	}
	
	
	
	
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
	for (int i = 0; i < exoModules_.size(); i++) if (exoModules_[i]->GetConnectStatus()) count++;

	if (count == exoModules_.size()) return true;
	else return false;
}
//---------------------------------------------------------------------------------------------------------
void exoSystem::StopAll() // Остановить всё.
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
	
	// Проверяем статус подключения всех модулей
	if (GetStutusConnectAllModules())
	{		
		for (int i = 0; i < ActuatorSize; i++) Actuator[i].PIDRegulator();
		/*
		// Выбор текущего решателя
		switch (СurrentSolver)
		{
		case TXTDATA:			 // Задающие генерируются из файла txt (матрица)

			break;
		case INVERSE_KINEMATICS: // Задающие генерируются из решения обратной задачи кинематики
			IK.GetCurrentAngles(Actuator, ActuatorSize, t);
			break;
		case PATTERNS_DATA:		 // Задающие генерируются из файла паттернов exolite txt
			sPatterns.GetCurrentAngles(Actuator, ActuatorSize, t / 100.0);
			//LOGD << "Time: " << t;
			break;
		default:
			StopAll();
			break;
		}
		*/
		
		SetPowerOn(1);
		//Actuator[8].SetTargetPosition(40);

		
		

		
		//Motors[9].SetPWM(15);
		//Motors[9].SetDirection(1, 0);

		//Actuator[9].SetTargetPosition(10);
		
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