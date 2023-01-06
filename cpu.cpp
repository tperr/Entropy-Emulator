#include "cpu.h"
#include <sstream>

// resets CPU, makes all registers 0
void CPU::reset()
{
	for (int i = 0; i < numOfRegs; i++)
		regValues[i] = 0;

	pc = 0;
	tc = 0;
	waitCounter = 1;
}
// sets register to specific value
void CPU::setRegister(string reg, unsigned char value)
{
	if (reg == "pc")
	{
		pc = value;
		return;
	}
	for (int i = 0; i < numOfRegs; i++)
	{
		if (regNames[i] == reg)
		{
			regValues[i] = value;
			break;
		}
	}
}
// outputs all registger values
void CPU::dump()
{
	printf("PC: 0x%02X\n", pc);

	for (int i = 0; i < numOfRegs; i++)
		printf("%s: 0x%02X\n", regNames[i].c_str(), regValues[i]);

	printf("TC: %d\n", tc);
	cout << endl;
}
// parses stuff for files
void CPU::parse(ifstream& file)
{
	string input;
	file >> input;
	if (input == "reset") // reset
		reset();
	else if (input == "set") // set register
	{
		file >> input; // skips over the "reg"
		file >> input;
		string reg = input;
		file >> input;
		setRegister(reg, stoi(input, nullptr, 16));
	}
	else if (input == "dump") // dumps
		dump();
}
// does any cycle work needed
void CPU::doCycleWork()
{
	if (waitCounter == 1)
	{
		iMemory->fetch(pc, 1, instructions, 0);
		decode(instructions);
	}
	else
		waitCounter--;

	tc++;

}
// decodes cpu commands
void CPU::decode(int* instructions)
{
	for (int j = 0; j < sizeof(instructions[j]) / sizeof(int); j++)
	{
		currentInstruction.instruction = instructions[j];
		currentInstruction.n = currentInstruction.instruction >> 17 & 0b111;
		currentInstruction.d = currentInstruction.instruction >> 14 & 0b111;
		currentInstruction.s = currentInstruction.instruction >> 11 & 0b111;
		currentInstruction.t = currentInstruction.instruction >> 8 & 0b111;
		currentInstruction.i = currentInstruction.instruction & 0b11111111;
		if (currentInstruction.instruction != 0)
		switch (currentInstruction.n)
		{
		case 5:
			lw();
			break;
		case 6:
			sw();
			break;
		case 0:
			add();
			break;
		case 1:
			addi();
			break;
		case 2:
			mul();
			break;
		case 3:
			inv();
			break;
		case 4:
			branch();
			break;
		case 7:
			halt();
			break;
		default:
			//printf("instruction = %x n = %x d = %x s = %x t = %x i = %x\n", currentInstruction.instruction, currentInstruction.n, currentInstruction.d, currentInstruction.s, currentInstruction.t, currentInstruction.i);
			break;
		}
		//printf("instruction = %x n = %x d = %x s = %x t = %x i = %x\n", currentInstruction.instruction, currentInstruction.n, currentInstruction.d, currentInstruction.s, currentInstruction.t, currentInstruction.i);

	}
}
// load word
void CPU::lw()
{
	//dump();
	//printf("lw reg[%d] = dmem[%d] is 0x%X\n", currentInstruction.t, currentInstruction.d, regValues[currentInstruction.t]);
	if (cache->fetch(regValues[currentInstruction.t], 1, regValues, currentInstruction.d))
		waitCounter = 1;
	else
		waitCounter = 5;
	pc++;
}
// save word
void CPU::sw()
{	
	//printf("sw dmem[%d] = reg[%d] is 0x%X\n", currentInstruction.t, currentInstruction.s, regValues[currentInstruction.s]);
	if (cache->store(regValues[currentInstruction.t], 1, regValues, currentInstruction.s))
		waitCounter = 1;
	else
		waitCounter = 5;
	pc++;
}
// add 2 registers
void CPU::add()
{

	regValues[currentInstruction.d] = regValues[currentInstruction.s] + regValues[currentInstruction.t];
	pc++;
}
// add immediate value
void CPU::addi()
{
	regValues[currentInstruction.d] = regValues[currentInstruction.s] + currentInstruction.i;
	pc++;
}
// multiply xxxx**** x ****xxxx parts of the byte
void CPU::mul()
{
	regValues[currentInstruction.d] = (regValues[currentInstruction.s] & 0b1111) * (regValues[currentInstruction.s] >> 4);
	pc++;
	waitCounter = 2;
}
// takes the inverse of the byte
void CPU::inv()
{
	regValues[currentInstruction.d] = ~regValues[currentInstruction.s] & 0xff;
	pc++;
}
// branch on equal
void CPU::beq()
{
	if (regValues[currentInstruction.s] == regValues[currentInstruction.t])
	{
		pc = currentInstruction.i;
		waitCounter = 2;
	}
	else
		pc++;
}
// branch on not equal
void CPU::bneq()
{
	if (regValues[currentInstruction.s] != regValues[currentInstruction.t])
	{
		pc = currentInstruction.i;
		waitCounter = 2;
	}
	else
		pc++;
}
// branch less than
void CPU::blt()
{
	if (regValues[currentInstruction.s] < regValues[currentInstruction.t])
	{
		pc = currentInstruction.i;
		waitCounter = 2;
	}
	else
		pc++;
}
// halt - stops cpu reading
void CPU::halt()
{
	waitCounter = -1;
	pc++;
}
// logic for which branch
void CPU::branch()
{
	switch (currentInstruction.d)
	{
		case 0:
			beq();
			break;
		case 1:
			bneq();
			break;
		case 2:
			blt();
			break;
		default:
			break;
	}
}
// constructors
CPU::CPU(IMemory* iMem, Cache* cac) : Device()
{
	deviceName = "cpu";
	iMemory = iMem;
	cache = cac;
	
	instructions = (int*) malloc(sizeof(int) * 1);
}
// wraps up stuff and things
void CPU::finish()
{
	free(instructions);
}