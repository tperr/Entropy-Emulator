#pragma once
#include "device.h"
#include "iMemory.h"
#include "cache.h"

// makes a CPU and makes it do things
class CPU: public Device
{
// class specific stuff
private:
	struct Instruction {
		int instruction;
		int n;
		int d;
		int s;
		int t;
		int i;
	} currentInstruction;
	int waitCounter = 1;
	short tc = 0;
	const int numOfRegs = 8;
	int* instructions;
	unsigned char regValues[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	char pc = 0;
	string regNames[8] = {"RA", "RB", "RC", "RD", "RE", "RF", "RG", "RH"};
	IMemory* iMemory;
	Cache* cache;

	void reset();
	void setRegister(string reg, unsigned char value);
	void dump();
	void doCycleWork();
	void finish();
	void lw();
	void sw();
	void add();
	void addi();
	void mul();
	void inv();
	void beq();
	void bneq();
	void blt();
	void halt();
	void branch();

	void decode(int* instruction);

	

// stuff used outside of CPU
public:
	void parse(ifstream& file);
	CPU(IMemory* iMem, Cache* cache);

};