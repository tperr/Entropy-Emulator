#include <iostream>
#include <fstream>

#include "clock.h"
#include "cpu.h"
#include "dMemory.h"
#include "iMemory.h"
#include "device.h"
#include "cache.h"
#include "io.h"

using namespace std;


// this is for the parser and sends it to the right place for processing
int main(int argc, char** argv)
{
	// declares some variables
	char newline;
	const int numOfDevices = 6;
	string input;
	ifstream file;
	DMemory dMem("memory");
	IMemory iMem("imemory"); // weird error when there was no parameter in constructor
	Cache cache(&dMem);
	CPU cptr(&iMem, &cache);
	IO io(&dMem);
	Device* devices[numOfDevices] = {&io, &cptr, &dMem, &iMem, &cache, &cptr};
	Clock clock(devices, numOfDevices);
	devices[1] = &clock;

	// bad args
	if (argc != 2)
	{
		printf("Error no file name or to many arguments\n");
		return 0;
	}
	
	// open
	file.open(argv[1]);

	// bad file
	if (!file.is_open())
	{
		printf("Error file couldn't be found, filepath: %s\n", argv[1]);
		return 0;
	}

	// goes through file and does logic with it
	while (file >> input)
		for (int i = 0; i < numOfDevices; i++)
			if (input == devices[i]->getName())
				devices[i]->parse(file); // specific devices parse
		
	// goes through devices and finishes the stuff
	for (int i = 0; i < numOfDevices; i++)
		devices[i]->finish();
	// donezo
	file.close();
}