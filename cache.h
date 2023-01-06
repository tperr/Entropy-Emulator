#pragma once
#include "device.h"
#include "dMemory.h"

using namespace std;

class Cache: public Device
{
private:
    bool isOn = false;
    int CLO = 0;
    unsigned char data[8];
    char flags[8];
    DMemory* mem;
	void dump();
	void reset();
public:
    void on();
    void off();
    bool fetch(unsigned int address, unsigned int count, unsigned char* dataPtr, unsigned int start);
    bool store(unsigned int address, unsigned int count, unsigned char* dataPtr, unsigned int start);
	void doCycleWork();
	void parse(ifstream& file);
	void finish();

    Cache(DMemory* mem);

};