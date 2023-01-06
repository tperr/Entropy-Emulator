#include "cache.h"

// outputs everything nicely
void Cache::dump()
{
    if (CLO == -1)
        printf("CLO        : 0x00\n");
    else
        printf("CLO        : 0x%02X\n", CLO);
    
    printf("cache data :");
    for (int i = 0; i < 8; i++)
        printf(" 0x%02X", data[i]);
    printf("\n");

    printf("Flags      :");
    for (int i = 0; i < 8; i++)
        printf("   %C ", flags[i]);
    printf("\n\n");

}
// resets cache
void Cache::reset()
{
    isOn = false;
    CLO = -1;
    for (int i = 0; i < 8; i++)
    {
        flags[i] = 'I';
        data[i] = 0;
    }
    
}
// turns cache on
void Cache::on()
{
    isOn = true;
}
// turns cache off
void Cache::off()
{
    isOn = false;
    for (int i = CLO; i < CLO + 8; i++)
        if (flags[i-CLO] == 'V')
            mem->store(i, 1, data, CLO);
}
// fetching a byte from memory
bool Cache::fetch(unsigned int address, unsigned int count, unsigned char* dataPtr, unsigned int start)
{
    // lw
    if (!isOn)
    {
        mem->fetch(address, count, dataPtr, start);
        return false;
    }
    if (address == 0xff)
    {
        for (int i = 0; i < 8; i++)
            flags[i] = 'I';
        CLO = -1;
        return true;
    }

    int line = address/8;
    if (line == CLO) // cache hit
    {
        dataPtr[start] = data[address%8];
        return true;
    }
    else // cache miss
    {
        CLO = line;
        mem->fetch(line, 8, data, 0);
        for (int i = 0; i < 8; i++)
            flags[i] = 'V';
        mem->fetch(address, count, dataPtr, start);
        return false;
    }

}
// storing a byte to memory
bool Cache::store(unsigned int address, unsigned int count, unsigned char* dataPtr, unsigned int start)
{ 
    // sw
    if (!isOn)
    {
        mem->store(address, count, dataPtr, start);
        return false;
    }
    if (address == 0xff)
    {
        bool ret = false;
        for (int i = 0; i < 8; i++)
        {
            if (flags[i] == 'W')
            {
                mem->store(CLO+i, 1, data, i);
                flags[i] = 'V';
                ret = true;
            }
        }
        return ret;
    }
    int line = address/8;

    if (line == CLO) // cache hit
    {
        data[address % 8] = dataPtr[start];
        flags[address % 8] = 'W';
        return true;
    }
    else // cache miss
    {
        for (int i = 0; i < 8; i++)
        {
            if (flags[i] == 'W')
                mem->store(CLO * 8 + i, 1, data, i);
            
            flags[i] = 'I';
        }
        CLO = line;
        data[address % 8] = dataPtr[start];
        flags[address % 8] = 'W';
        return false;

    }
}
// any cycle work that cache needs to do, its nothing
void Cache::doCycleWork()
{
}
// cache commands
void Cache::parse(ifstream& file)
{
    string input;
	file >> input;
	if (input == "reset")
		reset();
	else if (input == "on")
        on();
    else if (input == "off")
        off();
	else if (input == "dump")
		dump();
}
// wraps up everything
void Cache::finish()
{

}
// constructor
Cache::Cache(DMemory* mem) : mem(mem)
{
    reset();
    deviceName = "cache";
}