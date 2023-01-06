#include "clock.h"

// resets clock
void Clock::reset()
{
	count = 0;
}
// outputs current tick
void Clock::dump()
{
	cout << "Clock: " << count << endl;
}
// applies a work cycle to everything connected, recursive bc I like recursion and its easy in this case 
void Clock::tick(int amount)
{
	if (amount == 0) // base case, when no more ticks should be applied
		return;
	// does cycle work for all devices
	for (int i = 0; i < deviceCount; i++)
		deviceList[i]->doCycleWork();
	count++;
	tick(amount-1);
}
// parses for clock commands
void Clock::parse(ifstream& file)
{
	string input;
	file >> input;
	if (input == "reset") // reset clock cmd
		reset();
	else if (input == "tick") // tick for amount
	{
		int amount;

		file >> input;
		amount = atoi(input.c_str());
		tick(amount);
	}
	else if (input == "dump") // dump cmd
		dump();		
}
// does cycle work for clock (would there ever be any???)
void Clock::doCycleWork()
{
}
// constructor
Clock::Clock(Device* devices[], int size) : Device()
{
	deviceList = devices;
	deviceCount = size;
	deviceName = "clock";
}
// wraps clock up in a very nice wrapper and puts it in a bag to give to your parents for their bday
void Clock::finish()
{
}