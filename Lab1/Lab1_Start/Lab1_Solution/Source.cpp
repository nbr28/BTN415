#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include "Object.h"
using namespace W1;

void print(std::string msg, std::ofstream *ofs)
{
	std::cout << msg << std::endl;
	*ofs << msg << std::endl;
}

int main(int argc, char *argv[])
{
	std::string msgToWrite = "";
	std::ofstream ofs("Output.txt");
	if (!ofs.is_open())
		std::cout << "ERROR:  Failed to open file.  Screenshots will be required" << std::endl;

	int Max;

	if (argc < 2)
	{
		print("Invalid Number of Arguments", &ofs);
		return 0;
	}
	else
	{
		msgToWrite = "Number of Args: " + std::to_string(argc);
		print(msgToWrite, &ofs);
		msgToWrite = "Command Line: " + (std::string)argv[0] + " " + (std::string)argv[1];
		print(msgToWrite, &ofs);

		Max = atoi(argv[1]);
	}
	
	Object obj(Max, &ofs);

	srand(time(NULL));
	for (int x = 0; x < Max; x++)
	{
		int RandResult = rand() % 10 + 1;
		obj += RandResult;
	}

	obj.display();
	
	return 1;
}