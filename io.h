#include "device.h"
#include "dMemory.h"
#include <vector>
using namespace std;

class IO : public Device
{
private:
    struct instruction
    {
        string op;
        unsigned char address;
        unsigned char value;
    } instruction;
    unsigned char reg;
    int tick;
    int currentCmd;
    int performOp;
    string cmd[100];
    DMemory* mem;

	void dump();
	void reset();
    void load(string fileName);
    void read();
    void write();
// stuff that everything should use
public:
	void doCycleWork();
	void parse(ifstream& file);
	void finish();
    IO(DMemory* mem);
	
};