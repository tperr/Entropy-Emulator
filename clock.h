#pragma once
#include "device.h"

// a clock that allows for stuff to tick
class Clock: public Device
{
// stuff that goes inside the class
private:
	int count;
	Device** deviceList;
	int deviceCount;
	void reset();
	void tick(int amount);
	void dump();
	void doCycleWork();
	void finish();
// stuff that everything can access
public:
	void parse(ifstream& file);
	Clock(Device* devices[], int size);

};