#include "io.h"

void IO::dump()
{
    printf("IO Device: 0x%02X\n\n", reg);
}
void IO::reset()
{
    reg = 0;
    tick = 0;
    currentCmd = 0;
    performOp = -1;
}
void IO::load(string fileName)
{
    ifstream file;
    string input;
    file.open(fileName);
    int i = 0;
    while (getline(file, input))
    {
        cmd[i] = input;
        i++;
    }
}
void IO::doCycleWork()
{ 
    tick++;
    int spaceIdx = cmd[currentCmd].find(" ");
    string tc = cmd[currentCmd].substr(0, spaceIdx);
    if (tick == atoi(tc.c_str()))
    {
        cmd[currentCmd].erase(0, spaceIdx+1);
        spaceIdx = cmd[currentCmd].find(" ");
        string op = cmd[currentCmd].substr(0, spaceIdx);
        cmd[currentCmd].erase(0, spaceIdx+1);
        instruction.op = op;

        spaceIdx = cmd[currentCmd].find(" ");
        string address = cmd[currentCmd].substr(0, spaceIdx);

        instruction.address = stoi(address, nullptr, 16);

        if (op == "write")
        {
            cmd[currentCmd].erase(0, spaceIdx);
            instruction.value = stoi(cmd[currentCmd], nullptr, 16);
        }
        cmd[currentCmd].erase(0, spaceIdx);
        performOp = tick + 6;
        currentCmd++;
    }
    if (performOp == tick)
    {
        if (instruction.op == "read")
            read();
        else if (instruction.op == "write")
            write();
    }
}
void IO::parse(ifstream& file)
{
    string input;
	file >> input;
	if (input == "reset") // reset
		reset();
	else if (input == "load") // set register
	{
		file >> input;
        load(input);
	}
	else if (input == "dump") // dumps
		dump();
}
void IO::read()
{
    mem->fetch(instruction.address, 1, &reg, 0);
}
void IO::write()
{
    mem->store(instruction.address, 1, &instruction.value, 0);
}
void IO::finish()
{ }
IO::IO(DMemory* mem) : mem(mem)
{
   deviceName = "iodev"; 
   reset();
}