#pragma once

#define SIZE_BUFF 1024

#include <cstdlib>
#include <iostream>

namespace exo
{

struct exoItemPack
{
	std::string   header;
	uint8_t		  value_size;
	uint16_t      index;
};



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
		std::string write_pack()
		{
			return std::string(buff, set_item_index);
		}
		//-----------------------------------------------------------------------------------
		template < typename T >
		void set(std::string str, T& value)
		{
			for (int i = 0; i < buff[0]; i++)
			{
				if (ItemList[i].header == str)
				{
					for (int j = 0; j < ItemList[i].value_size; j++) buff[ItemList[i].index + j] = ((uint8_t*)&value)[j];
					return;
				}
			}

			buff[0] = buff[0] + 1;
			uint8_t header_size = str.size();
			buff[set_item_index] = header_size;
			for (int i = 0; i < header_size; i++) buff[set_item_index + i + 1] = str[i];

			uint8_t value_size = sizeof(T);
			buff[set_item_index + header_size + 1] = value_size;
			for (int i = 0; i < value_size; i++) buff[set_item_index + header_size + i + 2] = ((uint8_t*)&value)[i];


			exoItemPack Item;
			Item.header = str;
			Item.value_size = value_size;
			Item.index = set_item_index + header_size + 2;
			ItemList[buff[0] - 1] = Item;


			set_item_index += header_size + value_size + 2;
		}

		//-----------------------------------------------------------------------------------

		template < typename T >
		T get(const char *str, T default_)
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
		}
		//-----------------------------------------------------------------------------------
		~exoPackage()
		{

		}
		char buff[SIZE_BUFF];
	private:
		uint16_t set_item_index;
		uint16_t get_item_index;

		exoItemPack ItemList[100];
	};
}