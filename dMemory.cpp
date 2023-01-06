#include "dMemory.h"
#include <sstream>

// sets up memory
void DMemory::create(string size) 
{
	// creates new memory
	amount = stoi(size, nullptr, 16);
	memory = new unsigned char[amount];
	memFetchCounter = 0;
}
// resets all memory, makes itt not undefined (0)
void DMemory::reset()
{
	// makes all 0
	for (int i = 0; i < amount; i++)
		memory[i] = 0;
}
// outputs x amount of memory from x starting place
void DMemory::dump(int start, int count)
{
	// formatting the top to look nice
	cout << "Addr";
	for (int i = 0; i < 16; i++)
		printf(" %02X", (int) i);

	for (int i = 0; i < amount; i++)
	{
		if (i % 16 == 0 && i < start + count) printf("\n0x%02X", (int)i); // formats beginning
		if (i >= start && i < start + count) // outputs memory
			printf(" %02X", (int) memory[i]);
		else if (i >= start + count) // if its done printing
			break;
		else 
			printf("   "); // prints blank space for stuff before needed
	}
	cout << endl << endl;
}
// just dumps everything, mainly used for debgging
void DMemory::dumpStartToFinish()
{
	dump(0, amount);
}
// sets specific addresses to whatever you want
void DMemory::set(int start, int count, unsigned char mem[])
{
	for (int i = 0; i < count; i++)
		memory[start + i] = mem[i];
}
// does work, memory doesnt do much work
void DMemory::doCycleWork()
{
}
// fetches specific memory
void DMemory::fetch(unsigned int address, unsigned int count, unsigned char* dataPtr, unsigned int start)
{
	for (int i = 0; i < count; i++)
		dataPtr[i + start] = memory[address + i];
}
// stores specific memory
void DMemory::store(unsigned int address, unsigned int count, unsigned char* dataPtr, unsigned int start)
{
	for (int i = 0; i < count; i++)
		memory[address + i] = dataPtr[i+start];
}
// parses input for memory
void DMemory::parse(ifstream& file)
{
	string input;
	file >> input;
	if (input == "create") // create memory
	{
		file >> input;
		create(input);
	}
	else if (input == "reset") // resets memory
		reset();
	else if (input == "dump") // dumps memory
	{
		int from;
		int count;
		file >> input;
		from = stoi(input, nullptr, 16);
		file >> input; 
		count = stoi(input, nullptr, 16);

		dump(from, count);
	}
	else if (input == "set") // sets memory
	{
		file >> input;
		int start = stoi(input, nullptr, 16);

		file >> input;
		const int count = stoi(input, nullptr, 16);

		unsigned char * mem = (unsigned char*) malloc(sizeof(unsigned char) * count);
		for (int i = 0; i < count; i++)
		{
			file >> input;
			mem[i] = stoi(input, nullptr, 16);
		}
		set(start, count, mem);
		free(mem);
	}
}
// constructors
DMemory::DMemory(string name) : Device()
{
	deviceName = name;
}
DMemory::DMemory() : Device()
{
	DMemory("memory");
}
// wraps up anything necessary
void DMemory::finish()
{
}