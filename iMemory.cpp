#include "iMemory.h"
#include <sstream>

// sets up memory
void IMemory::create(string size)
{
	// creates new memory
	amount = stoi(size, nullptr, 16);
	memory = new int[amount];

}
// resets all memory, makes itt not undefined (0)
void IMemory::reset()
{
	// makes all 0
	for (int i = 0; i < amount; i++)
		memory[i] = 0;
}
// outputs x amount of memory from x starting place
void IMemory::dump(int start, int count)
{
	// formatting the top to look nice
	cout << "Addr";
	for (int i = 0; i < 8; i++)
		printf(" %5X", (int)i);

	for (int i = 0; i < amount; i++)
	{
		if (i % 8 == 0) printf("\n0x%02X", (int)i); // formats beginning
		if (i >= start && i < start + count) // outputs memory
			printf(" %05X", memory[i]);
		else if (i >= start + count) // if its done printing
			break;
		else
			printf("      "); // prints blank space for stuff before needed
	}
	cout << endl;
}
// just dumps everything, mainly used for debgging
void IMemory::dumpStartToFinish()
{
	dump(0, amount);
}
// sets specific addresses to whatever you want
void IMemory::set(int start, vector<int> mem)
{
	for (int i = 0; i < mem.size(); i++)
		memory[start + i] = mem[i];
}
// does work, memory doesnt do much work
void IMemory::doCycleWork()
{
}
// fetches specific memory
void IMemory::fetch(unsigned int address, unsigned int count, int* dataPtr, unsigned int start)
{
	for (int i = 0; i < count; i++)
		dataPtr[i+start] = memory[address + i];
}
// parses input for memory
void IMemory::parse(ifstream& file)
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
		file >> input; // skips file
		file >> input;
		ifstream setFile;
		setFile.open(input);
		if (!setFile.is_open())
		{
			printf("Error file couldn't be found, filepath: %s\n", input.c_str());
			throw invalid_argument("There was an error");
		}

		vector<int> mem;
		while (setFile >> input)
			mem.push_back(stoi(input, nullptr, 16));
		set(start, mem);
	}
}
// constructors
IMemory::IMemory(string name) : Device()
{
	deviceName = name;
	amount = 0;
	memory = new int[0];
}
IMemory::IMemory() : Device()
{
	IMemory("imemory");
}
// wraps up anything necessary
void IMemory::finish()
{
}