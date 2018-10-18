#ifndef SERVICEPATTERNS
#define SERVICEPATTERNS

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

namespace pattern
{

	int ArrayData[23][11];

	double Angle[10];

	//------------------------------------------------------------------------------
	void OpenPatterns(std::string file_)
	{
		std::ifstream file(file_);
		std::string line;

		int count_point = 0;

		while (std::getline(file, line))
		{
			if (line == "//Time:")
			{
				std::getline(file, line);
				ArrayData[count_point][0] = std::stoi(line);
			}
			if (line == "//Angles:")
			{
				for (int j = 1; j < 11; j++)
				{
					std::getline(file, line);
					ArrayData[count_point][j] = std::stoi(line);
				}
				count_point++;
			}
		}

	}
	//---------------------------------------------------------------
	double GetLine(double y1, double y2, double t1, double t2, double t)
	{
		double k = (y1 - y2) / (t1 - t2);
		double c = y1 - (k * t1);
		return k * t + c;
	}
	//---------------------------------------------------------------
	void GetCurrentAngles_Patterns(float ttime)
	{
		for (int i = 0; i < 23; i++)
		{
			//if (ttime >= ArrayData[22][0]) StopSimulation();

			if ((ttime >= ArrayData[i][0]) && (ttime <= ArrayData[i + 1][0]))
			{
				for (int j = 0; j < 10; j++)
					Angle[j] = GetLine(ArrayData[i][j + 1], ArrayData[i + 1][j + 1], ArrayData[i][0], ArrayData[i + 1][0], ttime);
				break;
			}
		}
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////
#endif
