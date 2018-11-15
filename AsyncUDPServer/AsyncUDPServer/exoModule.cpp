#define _WIN32_WINNT 0x0501

#include "exoModule.h"

#include <ctime>


exoModule::exoModule(boost::asio::io_context& io_context, std::string str_ip, std::string name)
	: Timer1(io_context, boost::posix_time::millisec(1000)), Timer_TimeOut(io_context)
{
	ip = boost::asio::ip::address::from_string(str_ip);
	name_ = name;
	Timer1.async_wait(boost::bind(&exoModule::Timer1_Hendle, this, boost::asio::placeholders::error, &Timer1));
	Timer_TimeOut.async_wait(boost::bind(&exoModule::Check_TimeOut, this));

	ConnectionStatus = false;
}
//---------------------------------------------------------------------------------------------
void exoModule::Timer1_Hendle(const boost::system::error_code& /*e*/, boost::asio::deadline_timer* t)
{
	t->expires_at(t->expires_at() + boost::posix_time::millisec(1000));
	t->async_wait(boost::bind(&exoModule::Timer1_Hendle, this, boost::asio::placeholders::error, t));

	FPS = NumberReceivPack;
	NumberReceivPack = 0;

}
//---------------------------------------------------------------------------------------------
int exoModule::GetFPS()
{
	return FPS;
}
//---------------------------------------------------------------------------------------------
void exoModule::Check_TimeOut()
{
	if (Timer_TimeOut.expires_at() <= boost::asio::deadline_timer::traits_type::now())
	{
		ConnectionStatus = false;
		Timer_TimeOut.expires_at(boost::posix_time::pos_infin);
	}
	// Put the actor back to sleep.
	Timer_TimeOut.async_wait(boost::bind(&exoModule::Check_TimeOut, this));
}
//----------------------------------------------------------------------------------------------

boost::asio::ip::address exoModule::GetIpAddress()
{
	return ip;
}
//----------------------------------------------------------------------------------------------

int exoModule::GetNumberReceivPack()
{
	return NumberReceivPack;
}
//----------------------------------------------------------------------------------------------

void exoModule::UpdatePack() 
{
	ConnectionStatus = true;
	NumberReceivPack++;
	//boost::recursive_mutex::scoped_lock lk(cs);
	Timer_TimeOut.expires_from_now(boost::posix_time::millisec(50));
	
}
//----------------------------------------------------------------------------------------------

std::string exoModule::GetName()
{
	return name_;
}
//----------------------------------------------------------------------------------------------

std::string exoModule::GetStringConnectStatus()
{
	if (ConnectionStatus == 0) return  "Disabled";
	else return "Connected";
}
//----------------------------------------------------------------------------------------------
bool exoModule::GetConnectStatus()
{
	return ConnectionStatus;
}
//----------------------------------------------------------------------------------------------
void exoModule::ModuleService()
{



}