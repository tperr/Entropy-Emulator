#pragma once
#include "device.h"

// class that is for memory stuff
class DMemory : public Device
{
// internal stuff
private:
	unsigned char * memory;
	int amount;
	int memFetchCounter = 0;
	int memStoreCounter = 0;
	void create(string size);
	void reset();
	void dump(int from, int count);
	void set(int start, int count, unsigned char mem[]);
	void doCycleWork();
	void finish();

// stuff that other stuff uses
public:
	void fetch(unsigned int address, unsigned int count, unsigned char* dataPtr, unsigned int start);
	void store(unsigned int address, unsigned int count, unsigned char* dataPtr, unsigned int start);
	void dumpStartToFinish();

	void parse(ifstream& file);
	DMemory(string name);
	DMemory();


};