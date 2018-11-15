#pragma once

#define SIZE_BUFF 1024

#include <cstdlib>

namespace exo
{

	
	class exoPackage
	{
	public:

		// thread-safe
		boost::recursive_mutex cs;		

		exoPackage()
		{
			buff[0] = 0;
			set_item_index = 1;
			get_item_index = 1;
		}
		//-----------------------------------------------------------------------------------
		void SetLength(uint16_t ln)
		{
			boost::recursive_mutex::scoped_lock lk(cs);
			set_item_index = ln;
		}
		//-----------------------------------------------------------------------------------
		uint16_t length()
		{
			boost::recursive_mutex::scoped_lock lk(cs);
			return set_item_index;
		}
		//-----------------------------------------------------------------------------------
		template < typename T >
		uint8_t init(const char *str)
		{
			boost::recursive_mutex::scoped_lock lk(cs);

			buff[0] = buff[0] + 1;
			uint8_t header_size = strlen(str);
			buff[set_item_index] = header_size;
			for (int i = 0; i < header_size; i++) buff[set_item_index + i + 1] = str[i];

			uint8_t value_size = sizeof(T);
			buff[set_item_index + header_size + 1] = value_size;

			uint8_t value_index = set_item_index + header_size + 2;

			for (int i = 0; i < value_size; i++) buff[value_index + i] = 0;

			set_item_index += header_size + value_size + 2;			
			return value_index;
		}
		//-----------------------------------------------------------------------------------
		template < typename T >
		void set2(uint8_t handle, T value)
		{
			boost::recursive_mutex::scoped_lock lk(cs);
			uint8_t value_size = sizeof(T);
			for (int i = 0; i < value_size; i++) buff[handle + i] = ((uint8_t*)&value)[i];
		}
		//-----------------------------------------------------------------------------------
		template < typename T >
		void set(const char *str, T& value)
		{
			boost::recursive_mutex::scoped_lock lk(cs);
			uint16_t index = GetIndexData(str);
			if (index != 0)
			{
				uint8_t value_size = sizeof(T);
				for (int i = 0; i < value_size; i++) buff[index+i] = ((uint8_t*)&value)[i];
				return;
			}						

			buff[0] = buff[0] + 1;
			uint8_t header_size = strlen(str);
			buff[set_item_index] = header_size;
			for (int i = 0; i < header_size; i++) buff[set_item_index + i + 1] = str[i];

			uint8_t value_size = sizeof(T);
			buff[set_item_index + header_size + 1] = value_size;
			for (int i = 0; i < value_size; i++) buff[set_item_index + header_size + i + 2] = ((uint8_t*)&value)[i];

			set_item_index += header_size + value_size + 2;
		}
		
		//-----------------------------------------------------------------------------------
		uint16_t GetIndexData(const char *str)
		{
			// ������ 
			for (int i = 0; i < buff[0]; i++)
			{
				uint8_t header_size = buff[get_item_index];


				//--------------
				uint8_t find_header_size = strlen(str);

				if (header_size == find_header_size)
				{
					uint8_t value_size = buff[get_item_index + header_size + 1];
					
					// ����������
					bool is = false;
					for (int h = 0; h < header_size; h++)
					{
						if (buff[get_item_index + h + 1] == str[h])
						{
							is = true;
						}
						else 
						{	
							is = false;
							break;
						}
					}

					if (is)
					{
						uint16_t ind = get_item_index + header_size + 2;
						get_item_index = 1;
						return ind;
					}
					else
					{
						get_item_index += header_size + value_size + 2;
					}

				}
				//--------------
				
				/*
				char *header = new char[header_size + 1];

				for (int h = 0; h < header_size; h++) header[h] = buff[get_item_index + h + 1];
				header[header_size] = '\0';

				uint8_t value_size = buff[get_item_index + header_size + 1];

				if (strcmp(str, header) == 0)
				{
					delete header;
					uint16_t ind = get_item_index + header_size + 2;
					get_item_index = 1;
					return ind;
				}
				else
				{
					delete header;
					get_item_index += header_size + value_size + 2;
				}
				*/
			}
			get_item_index = 1;
			return 0;
		}
		//-----------------------------------------------------------------------------------

		template < typename T >
		T get(const char *str, T default_)
		{
			boost::recursive_mutex::scoped_lock lk(cs);

			uint16_t index = GetIndexData(str);
			if (index != 0)
			{
				uint8_t value_size = buff[index-1];
				uint8_t read_buffer[8];
				for (int j = 0; j < value_size; j++) read_buffer[j] = buff[index + j];
				return *(T*)read_buffer;
			}
			else
			{
				int error = 1;
			}

			return default_;
		}
		//-----------------------------------------------------------------------------------
		~exoPackage()
		{

		}
		char buff[SIZE_BUFF];
	private:
		uint16_t set_item_index;
		uint16_t get_item_index;
	};
}