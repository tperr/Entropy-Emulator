#pragma once
#include "device.h"
#include <vector>

// class that is for memory stuff
class IMemory : public Device
{
	// internal stuff
private:
	int* memory;
	int amount;
	int memFetchCounter;
	void create(string size);
	void reset();
	void dump(int from, int count);
	void set(int start, vector<int> mem);
	void doCycleWork();
	void finish();
	void dumpStartToFinish();

	// stuff that other stuff uses
public:
	void fetch(unsigned int address, unsigned int count, int* dataPtr, unsigned int start);
	void parse(ifstream& file);
	IMemory();
	IMemory(string name);


};