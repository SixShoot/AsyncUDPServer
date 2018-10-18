#pragma once

#define SIZE_BUFF 1024

#include <cstdlib>

namespace exo
{

	
	class exoPackage
	{
	public:
		

		exoPackage()
		{
			buff[0] = 0;
			set_item_index = 1;
			get_item_index = 1;
		}
		//-----------------------------------------------------------------------------------
		uint16_t length()
		{
			return set_item_index;
		}
		//-----------------------------------------------------------------------------------
		template < typename T >
		uint8_t init(const char *str)
		{
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
			uint8_t value_size = sizeof(T);
			for (int i = 0; i < value_size; i++) buff[handle + i] = ((uint8_t*)&value)[i];
		}
		//-----------------------------------------------------------------------------------
		template < typename T >
		void set(const char *str, T& value)
		{
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
			// Чтение 
			for (int i = 0; i < buff[0]; i++)
			{
				uint8_t header_size = buff[get_item_index];

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
			}
			get_item_index = 1;
			return 0;
		}
		//-----------------------------------------------------------------------------------

		template < typename T >
		T get(const char *str, T default_)
		{
			/*
			// Чтение 
			for (int i = 0; i < buff[0]; i++)
			{
				uint8_t header_size = buff[get_item_index];

				char *header = new char[header_size + 1];

				for (int h = 0; h < header_size; h++) header[h] = buff[get_item_index + h + 1];
				header[header_size] = '\0';

				uint8_t value_size = buff[get_item_index + header_size + 1];

				if (strcmp(str, header) == 0)
				{
					delete header;
					uint8_t read_buffer[8];
					for (int j = 0; j < value_size; j++) read_buffer[j] = buff[get_item_index + header_size + 2 + j];
					get_item_index = 1;
					return *(T*)read_buffer;
				}
				else
				{
					delete header;
					get_item_index += header_size + value_size + 2;
				}
			}
			get_item_index = 1;
			return default_;
			*/
			uint16_t index = GetIndexData(str);
			if (index != 0)
			{
				uint8_t value_size = buff[index-1];
				uint8_t read_buffer[8];
				for (int j = 0; j < value_size; j++) read_buffer[j] = buff[index + j];
				return *(T*)read_buffer;
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